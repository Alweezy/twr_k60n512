/*
 * File: system_k60.c
 *
 * Copyright (c) 2013, 0xc0170
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 *
 */
#include "common.h"

int core_clk_mhz = 96;
int periph_clk_khz;

/**
 * \brief  Clock initialization (96 MHz)
 *
 * \param  void
 *
 * \return void
 */
void SystemInit(void)
{
  uint32_t temp_pfapr, i;

  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK
              | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK
              | SIM_SCGC5_PORTE_MASK);

  MCG_C2 = 0;
  SIM_SCGC4 |= SIM_SCGC4_LLWU_MASK;
  LLWU_CS |= LLWU_CS_ACKISO_MASK;
  MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);
  while (MCG_S & MCG_S_IREFST_MASK);
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2);

  MCG_C5 = MCG_C5_PRDIV(0x18);
  MCG_C6 = 0x0;

  temp_pfapr = FMC_PFAPR;

  FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
             | FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
             | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;

  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1)
              | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(3);

  for (i = 0 ; i < 4 ; i++);

  FMC_PFAPR = temp_pfapr;

  MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(24);
  while (!(MCG_S & MCG_S_PLLST_MASK));

  while (!(MCG_S & MCG_S_LOCK_MASK));
  MCG_C1 &= ~MCG_C1_CLKS_MASK;
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3);

  periph_clk_khz = 96000 / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24) + 1);
}
