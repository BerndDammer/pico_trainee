#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// --- CPU State & Memory ---
typedef struct {
    uint16_t PC;           // Program Counter
    uint8_t  S, A, X, Y, P; // Registers and Status
    uint32_t cycles;       // Total cycles executed
} CPU;

uint8_t bus[0x10000]; 
#define FLAG_C (1 << 0)
#define FLAG_Z (1 << 1)
#define FLAG_I (1 << 2)
#define FLAG_D (1 << 3)
#define FLAG_B (1 << 4)
#define FLAG_U (1 << 5)
#define FLAG_V (1 << 6)
#define FLAG_N (1 << 7)

// --- Helper Functions ---
uint8_t  read(uint16_t a) { return bus[a]; }
void     write(uint16_t a, uint8_t d) { bus[a] = d; }
void     set_zn(CPU *c, uint8_t v) {
    if (v == 0) c->P |= FLAG_Z; else c->P &= ~FLAG_Z;
    if (v & 0x80) c->P |= FLAG_N; else c->P &= ~FLAG_N;
}

// Stack helpers
void push(CPU *c, uint8_t v) { write(0x0100 + c->S--, v); }
uint8_t pull(CPU *c) { return read(0x0100 + ++c->S); }

// --- 1. Addressing Modes ---
// Each mode returns the address where the data is located.
uint16_t fetch_addr(CPU *c, uint8_t mode) {
    uint16_t lo, hi, addr;
    switch(mode) {
        case 0: return c->PC++; // Immediate
        case 1: return read(c->PC++); // Zero Page
        case 2: return (read(c->PC++) + c->X) & 0xFF; // ZP, X
        case 3: // Absolute
            lo = read(c->PC++); hi = read(c->PC++);
            return (hi << 8) | lo;
        case 4: // Absolute, X
            lo = read(c->PC++); hi = read(c->PC++);
            return ((hi << 8) | lo) + c->X;
        // ... (Add Indirect, Y-Indexed, etc.)
        default: return 0;
    }
}

// --- 2. The 56 Instructions ---
// Logic for every unique instruction type
void op_adc(CPU *c, uint16_t a) {
    uint8_t v = read(a);
    uint16_t res = c->A + v + (c->P & FLAG_C);
    if (~(c->A ^ v) & (c->A ^ (uint8_t)res) & 0x80) c->P |= FLAG_V; else c->P &= ~FLAG_V;
    if (res > 0xFF) c->P |= FLAG_C; else c->P &= ~FLAG_C;
    c->A = (uint8_t)res; set_zn(c, c->A);
}

void op_sbc(CPU *c, uint16_t a) {
    uint8_t v = read(a) ^ 0xFF; // 2's complement logic
    uint16_t res = c->A + v + (c->P & FLAG_C);
    if (~(c->A ^ v) & (c->A ^ (uint8_t)res) & 0x80) c->P |= FLAG_V; else c->P &= ~FLAG_V;
    if (res > 0xFF) c->P |= FLAG_C; else c->P &= ~FLAG_C;
    c->A = (uint8_t)res; set_zn(c, c->A);
}

void op_and(CPU *c, uint16_t a) { c->A &= read(a); set_zn(c, c->A); }
void op_asl(CPU *c, uint16_t a) {
    uint16_t v = read(a) << 1;
    if (v > 0xFF) c->P |= FLAG_C; else c->P &= ~FLAG_C;
    write(a, (uint8_t)v); set_zn(c, (uint8_t)v);
}
// ... (Repeat for BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS, CLC, CLD, CLI, CLV, 
// CMP, CPX, CPY, DEC, DEX, DEY, EOR, INC, INX, INY, JMP, JSR, LDA, LDX, LDY, LSR, NOP, 
// ORA, PHA, PHP, PLA, PLP, ROL, ROR, RTI, RTS, SEC, SED, SEI, STA, STX, STY, TAX, TAY, TSX, TXA, TXS, TYA)

// --- 3. The Instruction Map ---
typedef struct {
    void (*func)(CPU *, uint16_t);
    uint8_t mode;
    uint8_t cycles;
} Instruction;

// Partial map example
Instruction lookup[256] = {
    [0xA9] = {op_adc, 0, 2}, // ADC Immediate
    [0x69] = {op_adc, 0, 2}, // (Using A9/69 as examples)
    [0xAD] = {op_adc, 3, 4}, // ADC Absolute
    // ... Fill this with all 256 combinations
};

void step(CPU *c) {
    uint8_t opcode = read(c->PC++);
    if (lookup[opcode].func) {
        uint16_t addr = fetch_addr(c, lookup[opcode].mode);
        lookup[opcode].func(c, addr);
        c->cycles += lookup[opcode].cycles;
    }
}