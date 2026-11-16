// ==========================================================================
// Function at 1040:089a -> FUN_1040_089a
// ==========================================================================

/* WARNING: Removing unreachable block (ram,0x10400938) */
/* WARNING: Removing unreachable block (ram,0x1040096c) */

void __cdecl16far FUN_1040_089a(void)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  int *piVar4;
  int iVar5;
  bool bVar6;
  undefined4 uVar7;
  long lVar8;
  
  if (DAT_1068_45de != 0) {
    if (DAT_1068_257c != '\0') {
      uVar7 = GETCURRENTTIME();
      DAT_1068_45aa = (int)((ulong)uVar7 >> 0x10);
      piVar4 = (int *)&DAT_1068_45ac;
      for (iVar5 = 5; iVar5 != 0; iVar5 = iVar5 + -1) {
        if (*piVar4 != 0 || piVar4[1] != 0) {
          piVar4[4] = DAT_1068_45aa;
          piVar4[3] = (int)uVar7;
        }
        piVar4 = piVar4 + 5;
      }
      DAT_1068_45a8 = (int)uVar7;
      DAT_1068_257c = 0;
      return;
    }
    lVar8 = GETCURRENTTIME();
    piVar4 = (int *)&DAT_1068_45ac;
    for (iVar5 = 5; iVar5 != 0; iVar5 = iVar5 + -1) {
      while ((*piVar4 != 0 || piVar4[1] != 0 && (CONCAT22(piVar4[4],piVar4[3]) <= lVar8))) {
        uVar3 = piVar4[2];
        puVar1 = (uint *)(piVar4 + 3);
        uVar2 = *puVar1;
        *puVar1 = *puVar1 + uVar3;
        piVar4[4] = piVar4[4] + ((int)uVar3 >> 0xf) + (uint)CARRY2(uVar2,uVar3);
        (*(code *)*piVar4)(0x1160);
      }
      piVar4 = piVar4 + 5;
    }
    while (CONCAT22(DAT_1068_45aa,DAT_1068_45a8) < lVar8) {
      bVar6 = CARRY2(DAT_1068_45a8,DAT_1068_45de);
      DAT_1068_45a8 = DAT_1068_45a8 + DAT_1068_45de;
      DAT_1068_45aa = DAT_1068_45aa + ((int)DAT_1068_45de >> 0xf) + (uint)bVar6;
      if (DAT_1068_45e0 != 0) {
        DAT_1068_45e0 = DAT_1068_45e0 + -1;
      }
    }
  }
  return;
}


