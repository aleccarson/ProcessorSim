// CDA 3103 project Spring 2020
// Alec Carson

#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch(ALUControl)
    {
        // addition
        case 0:
            *ALUresult = A + B;
            break;

        // subtract
        case 1:
            *ALUresult = A - B;
            break;

        // signed less than
        case 2:
            if((signed)A < (signed)B)
                *ALUresult = 1;

            *ALUresult = 0;
            break;

        case 3:
            if(A < B)
                *ALUresult = 1;

            *ALUresult = 0;
            break;
        
        // bitwise and
        case 4:
            *ALUresult = (A & B);
            break;

        // bitwise or
        case 5:
            *ALUresult = (A | B);
            break;

        // A < 0
        case 6:
            if(A < 0)
                *ALUresult = 1;

            *ALUresult = 0;
            break;

        case 7:
            *ALUresult = ~A;
            break;

        
    }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    // return 1 if halt condition is met
    if(PC % 4 != 0)
        return 1;

    // get correct addr and set instuction accordingly 
    *instruction = Mem[PC >> 2];

    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // 31-26(op)
    *op = (instruction & 0xfc000000) >> 26;
    // 25-21 (r1)
    *r1 = (instruction & 0x03e00000) >> 21;
    // 20-16 (r2)
    *r2 = (instruction & 0x001f0000) >> 16;
    // 15-11 (r3)
    *r3 = (instruction & 0x0000f800) >> 11;
    // 5-0 (funct)
    *funct = instruction & 0x0000003f;
    // 15-0 (offset)
    *offset = instruction & 0x0000ffff;
    // 25-0 (jsec)
    *jsec = instruction & 0x03ffffff;
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{   
    switch(op)
    {
        default:
            return 1;

        // REgister type
        case 0:
            controls->MemtoReg = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->ALUOp = 7;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->RegDst = 1;
            controls->RegWrite = 1;

            break;
        
        // Immediate type (beq)
        case 4:
            controls->MemtoReg = 2;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->ALUOp = 1;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->RegDst = 2;
            controls->RegWrite = 0;

            break;
        
        // immediate type (addi)
        case 8:
            controls->MemtoReg = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->ALUOp = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->RegDst = 0;
            controls->RegWrite = 1;

            break;
    
        //immediate type slt
        case 10:
            controls->MemtoReg = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->ALUOp = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->RegDst = 1;
            controls->RegWrite = 1;

            break;

        //immediate tyope (sltu)
        case 11:
            controls->MemtoReg = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->ALUOp = 3;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->RegDst = 1;
            controls->RegWrite = 1;

            break;

        //immediate type lui
        case 15:
            controls->MemtoReg = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->ALUOp = 6;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->RegDst = 0;
            controls->RegWrite = 1;

            break;
    
        //immediate type lw
        case 35:
            controls->MemtoReg = 1;
            controls->MemRead = 1;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->ALUOp = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->RegDst = 0;
            controls->RegWrite = 1;

            break;

        //immediate type sw
        case 43:
            controls->MemtoReg = 2;
            controls->MemRead = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->ALUOp = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->RegDst = 2;
            controls->RegWrite = 0;

            break;
        
        // J type (jump)
        case 2:
            controls->MemtoReg = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->ALUOp = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->RegDst = 0;
            controls->RegWrite = 0;

            break;
    }

    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // get values from reg array 
    *data1 = Reg[r1], *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // check if 16th bit is 1 meaning it's neg (extend 1s)
    if(offset >> 15 & 1)
        *extended_value = offset | 0xffff0000;

    // extend 0s otherwise
    *extended_value = offset & 0x0000ffff;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    // I type ops
    if(ALUSrc == 1)
        ALU(data1, extended_value, ALUOp, ALUresult, Zero);

    if(ALUSrc == 0)
    {
    // r type instructions'
        switch(funct)
        {
            // halt/don't care
            default:
                return 1;

            //addition
            case 32:
                ALUOp = 0;
                break;
            
            // sbutraction
            case 34:
                ALUOp = 1;
                break;

            // slt signed
            case 42:
                ALUOp = 2;
                break;

            //slt unsigned
            case 43:
                ALUOp = 3;
                break;

            // bitwise AND
            case 36:
                ALUOp = 4;
                break;
            
            //bitwise OR
            case 37:
                ALUOp = 5;
                break;

            // slr  
            case 4:
                ALUOp = 6;
                break;

            // bitwie nor
            case 39:
                ALUOp = 7;
                break;

        }
        //ALU function
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
  if((MemRead == 1 || MemWrite == 1) && ALUresult % 4 != 0)
    return 1;

    if(MemRead == 1)
        *memdata = Mem[ALUresult >> 2];
    
    if(MemWrite == 1)
        Mem[ALUresult >> 2] = data2;

    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
        if(RegWrite == 1)
        {
            // to r3
            if(MemtoReg == 1 && RegDst == 1)
                Reg[r3] = memdata;

            // mem to reg
            if(MemtoReg == 1 && RegDst == 0)
                Reg[r2] = memdata;

            // store in r2
            if(MemtoReg == 0 && RegDst == 0)
                Reg[r2] = ALUresult;

            // store in r3
            if(MemtoReg == 0 && RegDst == 1)
                Reg[r3] = ALUresult;
        }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    // pc inc by 4 after each instruction
    *PC += 4;
    //branching and got 0
    if(Zero == 1 && Branch == 1)
        *PC += extended_value << 2;

    if(Jump == 1)
        *PC = (jsec << 2) | (*PC | 0xf0000000);
}

