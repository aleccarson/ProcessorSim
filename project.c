// CDA 3103 project Spring 2020
// Alec Carson

#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    // Addition
    if(ALUcontrol == 0)
        *ALUresult = A + B;

    // Subtraction
    if(ALUcontrol == 1)
        *ALUresult = A - B;

    // signed Less than   
    if(ALUcontrol == 2)
        {
            if((signed)A < (signed)B)
                *ALUresult = 1;
            else
                *ALUresult = 0;            
        }

    // Less than unsigned   
    if(ALUcontrol == 3)
        {
            if(A < B)
                *ALUresult = 1;
            else
                *ALUresult = 0;            
        }

        // bitwise AND
        if(ALUcontrol == 4)
            *ALUresult = (A & B);

    // bitwise OR
    if(ALUcontrol == 5)
        *ALUresult = (A | B);

    // A < 0
    if(ALUcontrol == 6)
    {
        if (A < 0)
            *ALUresult = 1;

        *ALUresult = 0;
    }

    // bitwse NOT
    if(ALUcontrol == 7)
        *ALUresult = ~A;

    if(*ALUresult == 0)
        *Zero = 1;

    *Zero = 0;
    
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
    *r1 = (instruction & 0x03e00000) >> 32;
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
    // REgister type
    if(op == 0)
    {
        controls->MemtoReg = 0;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->ALUOp = 7;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->RegDst = 1;
        controls->RegWrite = 1;
    }

    // Immediate type (beq)
    if(op == 4)
    {
        controls->MemtoReg = 2;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->ALUOp = 1;
        controls->Jump = 0;
        controls->Branch = 1;
        controls->RegDst = 2;
        controls->RegWrite = 0;
    }

    // immediate type (addi)
    if(op == 8)
    {
        controls->MemtoReg = 0;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->ALUOp = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
    }

    //immediate type slt
    if(op == 10)
    {
        controls->MemtoReg = 0;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->ALUOp = 2;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->RegDst = 1;
        controls->RegWrite = 1;
    }

    //immediate tyope (sltu)
    if(op == 11)
    {
        controls->MemtoReg = 0;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->ALUOp = 3;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->RegDst = 1;
        controls->RegWrite = 1;
    }

    //immediate type lui
    if(op == 15)
    {
        controls->MemtoReg = 0;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->ALUOp = 6;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
    }

    //immediate type lw
    if(op == 35)
    {
        controls->MemtoReg = 1;
        controls->MemRead = 1;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->ALUOp = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->RegDst = 0;
        controls->RegWrite = 1;
    }

    //immediate type sw
    if(op == 43)
    {
        controls->MemtoReg = 2;
        controls->MemRead = 0;
        controls->MemWrite = 1;
        controls->ALUSrc = 1;
        controls->ALUOp = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->RegDst = 2;
        controls->RegWrite = 0;
    }

    // J type (jump)
    if(op == 2)
    {
        controls->MemtoReg = 0;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->ALUOp = 0;
        controls->Jump = 1;
        controls->Branch = 0;
        controls->RegDst = 0;
        controls->RegWrite = 0;
    }
    // halt if invalid opcode
    return 1;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

