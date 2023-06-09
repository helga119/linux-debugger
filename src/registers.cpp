#ifndef MINIDBG_REGISTERS_HPP
#define MINIDBG_REGISTERS_HPP


#include <sys/user.h>
#include <algorithm>

// Enum class representing different registers
namespace minidbg{
enum class reg {
    rax, rbx, rcx, rdx,
    rdi, rsi, rbp, rsp,
    r8, r9, r10, r11,
    r12, r13, r14, r15,
    rip, rflags, cs,
    orig_rax, fs_base,
    gs_base,
    fs, gs, ss, ds, es
};

// Total number of registers
static constexpr std::size_t n_registers = 27;

// Structure representing a register descriptor
struct reg_descriptor {
    reg r;              // Register
    int dwarf_r;        // DWARF register number
    std::string name;   // Register name
};

// Array of register descriptors
static const std::array<reg_descriptor, n_registers> g_register_descriptors{{
    {reg::r15, 15, "r15"},
    {reg::r14, 14, "r14"},
    {reg::r13, 13, "r13"},
    {reg::r12, 12, "r12"},
    {reg::rbp, 6, "rbp"},
    {reg::rbx, 3, "rbx"},
    {reg::r11, 11, "r11"},
    {reg::r10, 10, "r10"},
    {reg::r9, 9, "r9"},
    {reg::r8, 8, "r8"},
    {reg::rax, 0, "rax"},
    {reg::rcx, 2, "rcx"},
    {reg::rdx, 1, "rdx"},
    {reg::rsi, 4, "rsi"},
    {reg::rdi, 5, "rdi"},
    {reg::orig_rax, -1, "orig_rax"},
    {reg::rip, -1, "rip"},
    {reg::cs, 51, "cs"},
    {reg::rflags, 49, "rflags"},
    {reg::rsp, 7, "rsp"},
    {reg::ss, 52, "ss"},
    {reg::fs_base, 58, "fs_base"},
    {reg::gs_base, 59, "gs_base"},
    {reg::ds, 53, "ds"},
    {reg::es, 50, "es"},
    {reg::fs, 54, "fs"},
    {reg::gs, 55, "gs"},
}};

uint64_t get_register_value(pid_t pid, reg r) {
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
    auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                           [r](auto&& rd) { return rd.r == r; });

    // make safer later:
    // return *(reinterpret_cast<uint64_t*>(&regs) + (it - begin(g_register_descriptors)));

    // Safer
    switch (r) {
        case reg::r15:
            return regs.r15;
        case reg::r14:
            return regs.r14;
        case reg::r13:
            return regs.r13;
        case reg::r12:
            return regs.r12;
        case reg::rbp:
            return regs.rbp;
        case reg::rbx:
            return regs.rbx;
        case reg::r11:
            return regs.r11;
        case reg::r10:
            return regs.r10;
        case reg::r9:
            return regs.r9;
        case reg::r8:
            return regs.r8;
        case reg::rax:
            return regs.rax;
        case reg::rcx:
            return regs.rcx;
        case reg::rdx:
            return regs.rdx;
        case reg::rsi:
            return regs.rsi;
        case reg::rdi:
            return regs.rdi;
        case reg::orig_rax:
            return regs.orig_rax;
        case reg::rip:
            return regs.rip;
        case reg::cs:
            return regs.cs;
        case reg::rflags:
            return regs.eflags;
        case reg::rsp:
            return regs.rsp;
        case reg::ss:
            return regs.ss;
        case reg::fs_base:
            return regs.fs_base;
        case reg::gs_base:
            return regs.gs_base;
        case reg::ds:
            return regs.ds;
        case reg::es:
            return regs.es;
        case reg::fs:
            return regs.fs;
        case reg::gs:
            return regs.gs;
    }
}

uint64_t get_register_value(pid_ pid, reg r){
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, nullptr,&regs);
    auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                           [r](auto&& rd){return rd.r == r; });
                           return *(reinterpret_cast<uint64_t*>(&regs)+(it -begin(g_register_descriptors)));
}


void set_register_value(pid_t pid, reg r, uint64_t value) {
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
    auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                           [r](auto&& rd) { return rd.r == r; });

    *(reinterpret_cast<uint64_t*>(&regs) + (it - begin(g_register_descriptors))) = value;
    ptrace(PTRACE_SETREGS, pid, nullptr, &regs);
}

uint64_t get_register_value_from_dwarf_register(pid_t pid, unsigned regnum) {
    auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                           [regnum](auto&& rd) { return rd.dwarf_r == regnum; });

    if (it == end(g_register_descriptors)) {
        throw std::out_of_range{"Unknown dwarf register"};
    }

    return get_register_value(pid, it->r);
}

std::string get_register_name(reg r) {
    auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                           [r](auto&& rd) { return rd.r == r; });
    return it->name;
}

reg get_register_from_name(const std::string& name) {
    auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                           [name](auto&& rd) { return rd.name == name; });
    return it->r;
}
}
#endif