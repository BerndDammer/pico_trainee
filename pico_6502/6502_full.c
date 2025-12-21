#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// --- CPU State ---
typedef struct {
    uint16_t PC;
    uint8_t  A, X, Y, S, P;
    uint64_t cycles;
} CPU;

uint8_t mem[0x10000];

// Status Flags
enum { C = 1<<0, Z = 1<<1, I = 1<<2, D = 1<<3, B = 1<<4, U = 1<<5, V = 1<<6, N = 1<<7 };

// --- Memory Access ---
uint8_t  peek(uint16_t a) { return mem[a]; }
void     poke(uint16_t a, uint8_t d) { mem[a] = d; }
uint16_t fetch16(uint16_t a) { return peek(a) | (peek(a + 1) << 8); }

// --- Helpers ---
void set_zn(CPU *cpu, uint8_t v) {
    cpu->P = (v == 0) ? (cpu->P | Z) : (cpu->P & ~Z);
    cpu->P = (v & 0x80) ? (cpu->P | N) : (cpu->P & ~N);
}

void push(CPU *cpu, uint8_t v) { poke(0x0100 + cpu->S--, v); }
uint8_t pull(CPU *cpu) { return peek(0x0100 + ++cpu->S); }

// --- Addressing Modes ---
typedef struct { uint16_t addr; bool page_cross; } AddrRes;

AddrRes get_addr(CPU *cpu, uint8_t mode) {
    uint16_t lo, hi, addr;
    switch(mode) {
        case 0: return (AddrRes){ cpu->PC++, false }; // Immediate
        case 1: return (AddrRes){ peek(cpu->PC++), false }; // Zero Page
        case 2: return (AddrRes){ (uint8_t)(peek(cpu->PC++) + cpu->X), false }; // ZP,X
        case 3: // Absolute
            addr = fetch16(cpu->PC); cpu->PC += 2;
            return (AddrRes){ addr, false };
        case 4: // Absolute,X
            lo = fetch16(cpu->PC); cpu->PC += 2; addr = lo + cpu->X;
            return (AddrRes){ addr, (addr & 0xFF00) != (lo & 0xFF00) };
        case 5: // Indirect,Y
            lo = peek(cpu->PC++); hi = peek(lo) | (peek((uint8_t)(lo + 1)) << 8);
            addr = hi + cpu->Y;
            return (AddrRes){ addr, (addr & 0xFF00) != (hi & 0xFF00) };
        default: return (AddrRes){0, false};
    }
}

// --- Instructions ---
#define OP_LDA cpu->A = peek(res.addr); set_zn(cpu, cpu->A);
#define OP_LDX cpu->X = peek(res.addr); set_zn(cpu, cpu->X);
#define OP_LDY cpu->Y = peek(res.addr); set_zn(cpu, cpu->Y);
#define OP_STA poke(res.addr, cpu->A);
#define OP_STX poke(res.addr, cpu->X);
#define OP_STY poke(res.addr, cpu->Y);

void adc(CPU *cpu, uint16_t addr) {
    uint8_t v = peek(addr);
    uint16_t sum = cpu->A + v + (cpu->P & C);
    if (~(cpu->A ^ v) & (cpu->A ^ (uint8_t)sum) & 0x80) cpu->P |= V; else cpu->P &= ~V;
    if (sum > 0xFF) cpu->P |= C; else cpu->P &= ~C;
    cpu->A = (uint8_t)sum; set_zn(cpu, cpu->A);
}

void branch(CPU *cpu, bool cond) {
    int8_t rel = (int8_t)peek(cpu->PC++);
    if (cond) {
        cpu->cycles++;
        if ((cpu->PC & 0xFF00) != ((cpu->PC + rel) & 0xFF00)) cpu->cycles++;
        cpu->PC += rel;
    }
}

// --- The Big Execution Switch ---
void step(CPU *cpu) {
    uint8_t opcode = peek(cpu->PC++);
    AddrRes res;

    switch(opcode) {
        // LDA
        case 0xA9: res = get_addr(cpu, 0); OP_LDA; cpu->cycles += 2; break;
        case 0xA5: res = get_addr(cpu, 1); OP_LDA; cpu->cycles += 3; break;
        case 0xAD: res = get_addr(cpu, 3); OP_LDA; cpu->cycles += 4; break;
        
        // STA
        case 0x85: res = get_addr(cpu, 1); OP_STA; cpu->cycles += 3; break;
        case 0x8D: res = get_addr(cpu, 3); OP_STA; cpu->cycles += 4; break;

        // ADC
        case 0x69: res = get_addr(cpu, 0); adc(cpu, res.addr); cpu->cycles += 2; break;
        
        // Arithmetic / Inc / Dec
        case 0xE8: cpu->X++; set_zn(cpu, cpu->X); cpu->cycles += 2; break; // INX
        case 0xCA: cpu->X--; set_zn(cpu, cpu->X); cpu->cycles += 2; break; // DEX
        case 0xC8: cpu->Y++; set_zn(cpu, cpu->Y); cpu->cycles += 2; break; // INY
        case 0x88: cpu->Y--; set_zn(cpu, cpu->Y); cpu->cycles += 2; break; // DEY

        // Branching
        case 0xF0: branch(cpu, cpu->P & Z); cpu->cycles += 2; break; // BEQ
        case 0xD0: branch(cpu, !(cpu->P & Z)); cpu->cycles += 2; break; // BNE
        case 0x90: branch(cpu, !(cpu->P & C)); cpu->cycles += 2; break; // BCC
        case 0xB0: branch(cpu, cpu->P & C); cpu->cycles += 2; break; // BCS

        // Stack & Control
        case 0x20: // JSR
            res = get_addr(cpu, 3);
            push(cpu, ((cpu->PC - 1) >> 8) & 0xFF);
            push(cpu, (cpu->PC - 1) & 0xFF);
            cpu->PC = res.addr; cpu->cycles += 6; break;
        case 0x60: // RTS
            cpu->PC = (pull(cpu) | (pull(cpu) << 8)) + 1; cpu->cycles += 6; break;
        case 0xEA: cpu->cycles += 2; break; // NOP

        default: printf("Unimplemented Opcode: %02X\n", opcode); break;
    }
}

int main() {
    CPU cpu = { .PC = 0x8000, .S = 0xFF, .P = U | I };
    
    // Tiny Program: LDA #$01, INX, BNE -3 (loop 255 times), STA $0200
    uint8_t prog[] = { 0xA9, 0x01, 0xE8, 0xD0, 0xFD, 0x8D, 0x00, 0x02 };
    for(int i=0; i<sizeof(prog); i++) mem[0x8000+i] = prog[i];

    printf("Running...\n");
    for(int i=0; i<1000; i++) step(&cpu); // Run enough steps to finish loop

    printf("Final A: %02X, X: %02X, RAM[0200]: %02X\n", cpu.A, cpu.X, mem[0x0200]);
    return 0;
}