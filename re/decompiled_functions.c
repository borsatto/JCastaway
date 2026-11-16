// ==========================================================================
// Function at 1018:1771 -> FUN_1018_1771
// ==========================================================================

void __cdecl16far FUN_1018_1771(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined2 *puVar4;
  undefined2 uVar5;
  int unaff_BP;
  bool bVar6;
  undefined2 local_14;
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  int local_c;
  undefined4 local_a;
  int local_6;
  undefined2 local_4;
  int iStack_2;
  
  iStack_2 = unaff_BP + 1;
  local_4 = 0x1068;
  if (DAT_1068_3360 == '\0') {
    DAT_1068_354f = 0;
    DAT_1068_354d = 0;
    DAT_1068_3360 = '\x01';
    iVar2 = FUN_1000_0cff((char *)s_RESOURCE_MAP_1068_1cbc,0x1cc9);
    if (iVar2 != 0) {
      FUN_1000_0df7((undefined1 *)&DAT_1068_1cb8,4,1,iVar2);
      FUN_1000_0df7(&local_c,2,1,iVar2);
      DAT_1068_3354 = DAT_1068_3354 + local_c;
      iVar3 = DAT_1068_3354 - local_c;
      while (iVar3 = iVar3 + 1, iVar3 <= DAT_1068_3354) {
        local_6 = iVar3 * 0x1c + 0x3365;
        FUN_1000_0df7(local_6,0xd,1,iVar2);
        FUN_1000_0df7(&local_c,2,1,iVar2);
        uVar5 = 0;
        puVar4 = (undefined2 *)FUN_1030_199a((local_c + 1) * 8,0,0,1);
        local_a = (undefined2 *)CONCAT22(uVar5,puVar4);
        *(undefined2 *)(local_6 + 0x1a) = uVar5;
        *(undefined2 *)(local_6 + 0x18) = puVar4;
        *(int *)(local_6 + 0xe) = iVar3;
        while (iVar1 = local_c + -1, bVar6 = local_c != 0, local_c = iVar1, bVar6) {
          FUN_1000_0df7(&local_10,4,1,iVar2);
          FUN_1000_0df7(&local_14,4,1,iVar2);
          uVar5 = (undefined2)((ulong)local_a >> 0x10);
          puVar4 = (undefined2 *)local_a;
          puVar4[1] = local_e;
          *local_a = local_10;
          puVar4[3] = local_12;
          puVar4[2] = local_14;
          local_a = (undefined2 *)CONCAT22(uVar5,puVar4 + 4);
        }
      }
      FUN_1000_0a3e(iVar2);
    }
  }
  return;
}


// ==========================================================================
// Function at 1008:0fcd -> SCREENSAVERPROC
// ==========================================================================

undefined4 __stdcall16far
SCREENSAVERPROC(int param_1,int param_2,int param_3,uint param_4,undefined2 param_5)

{
  undefined2 uVar1;
  int iVar2;
  int unaff_BP;
  undefined2 unaff_CS;
  undefined2 unaff_SS;
  undefined4 uVar3;
  undefined2 uVar4;
  int *piVar5;
  undefined1 local_46 [18];
  undefined1 local_34 [4];
  int local_30;
  int local_2e;
  int local_2c;
  int local_2a;
  uint local_14;
  int local_12;
  int local_10;
  int local_e;
  int local_c;
  int local_a;
  int local_8;
  int local_6;
  undefined2 local_4;
  int iStack_2;
  
  iStack_2 = unaff_BP + 1;
  local_4 = 0x1068;
  if (param_4 == 0x14) {
    GETCLIENTRECT(unaff_CS,&local_c,unaff_SS);
    piVar5 = &local_c;
    uVar4 = 4;
    uVar1 = GETSTOCKOBJECT(0x1160,4,piVar5);
    FILLRECT(0x1160,uVar1,uVar4,piVar5);
    local_e = (DAT_1068_2562 - DAT_1068_410c) / 2;
    local_10 = (DAT_1068_2564 - DAT_1068_410a) / 2;
    FUN_1060_007e(local_c + local_e,local_a + local_10,local_8 - local_c,local_6 - local_a,1,
                  DAT_1068_1eb2,DAT_1068_1eb4,0,0,0);
    return 1;
  }
  if (param_4 < 0x15) {
    if (param_4 == 5) {
      if ((param_3 == 0) || (param_3 == 2)) {
        DAT_1068_410c = param_1;
        DAT_1068_410a = param_2;
        INVALIDATERECT(unaff_CS,1,0,0,param_5);
        unaff_CS = 0x1160;
        UPDATEWINDOW(0x1160,param_5);
      }
    }
    else if (param_4 < 6) {
      if (param_4 == 1) {
        FUN_1008_0c5c();
        SETTIMER(unaff_CS,0,0,0x32,1);
        unaff_CS = 0x1160;
      }
      else if (param_4 == 2) {
        uVar1 = param_5;
        KILLTIMER(unaff_CS,1);
        DAT_1068_2d84 = param_5;
        if (DAT_1068_2d8c != (code *)0x0 || DAT_1068_2d8e != 0) {
          (*DAT_1068_2d8c)(0x1160,uVar1);
        }
        DAT_1068_2d8e = 0;
        DAT_1068_2d8c = (code *)0x0;
        FUN_1008_0dc8();
        unaff_CS = 0x1160;
      }
    }
    else if (param_4 == 0xf) {
      DAT_1068_2d84 = param_5;
      BEGINPAINT(unaff_CS,local_34,unaff_SS,param_5);
      local_e = (DAT_1068_2562 - DAT_1068_410c) / 2;
      local_10 = (DAT_1068_2564 - DAT_1068_410a) / 2;
      FUN_1060_007e(local_30 + local_e,local_2e + local_10,local_2c - local_30,local_2a - local_2e,1
                    ,DAT_1068_1eb2,DAT_1068_1eb4,0,0,0);
      unaff_CS = 0x1160;
      ENDPAINT(0x1060,local_34,unaff_SS);
    }
  }
  else if (param_4 == 0x112) {
    if (param_3 == -0xec0) {
      return 1;
    }
  }
  else if (param_4 == 0x113) {
    if (DAT_1068_2d86 == 0) {
      DAT_1068_2d84 = param_5;
      FUN_1040_089a();
      if (DAT_1068_2d90 != (code *)0x0 || DAT_1068_2d92 != 0) {
        (*DAT_1068_2d90)(0x1040);
      }
      while( true ) {
        unaff_CS = 0x1160;
        iVar2 = PEEKMESSAGE(0x1040,0,0,0,0,local_46,unaff_SS);
        if (iVar2 != 0) break;
        FUN_1040_089a();
      }
    }
  }
  else if (param_4 == 0x400) {
    if (DAT_1068_00ca == 0) {
      iVar2 = -1;
      local_14 = FUN_1030_199a(0xffff,0xffff,0,0);
      local_12 = iVar2;
      if ((iVar2 < 0x27) && ((iVar2 < 0x26 || (local_14 < 0x25a0)))) {
        FUN_1020_081c(0x1d9,local_14,iVar2);
        DAT_1068_0054 = 0x3ea;
        DAT_1068_0056 = 0x30;
        DESTROYWINDOW(0x1020,param_5);
        return 0;
      }
      if ((iVar2 < 0x32) && ((iVar2 < 0x31 || (local_14 < 0x9750)))) {
        DAT_1068_1e22 = 0;
        FUN_1020_081c(0x206,local_14,iVar2);
      }
      DAT_1068_00ca = 1;
      DAT_1068_2d84 = param_5;
      iVar2 = FUN_1008_143c();
      if (iVar2 == 0) {
        DAT_1068_0054 = 0x3e9;
        DAT_1068_0056 = 0x30;
        DESTROYWINDOW(0x1008,param_5);
      }
      DAT_1068_2d82 = 0;
    }
    return 0;
  }
  if (DAT_1068_00c8 != 0) {
    uVar3 = DEFWINDOWPROC(unaff_CS,param_1,param_2,param_3,param_4);
    return uVar3;
  }
  uVar3 = FUN_1008_0221(param_1,param_2,param_3,param_4,param_5);
  return uVar3;
}


// ==========================================================================
// Function at 1008:034f -> SCREENSAVERCONFIGUREDIALOG
// ==========================================================================

undefined2 __stdcall16far
SCREENSAVERCONFIGUREDIALOG
          (undefined2 param_1,undefined2 param_2,uint param_3,int param_4,undefined2 param_5)

{
  undefined2 uVar1;
  undefined2 unaff_CS;
  long lVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  
  if (param_4 == 0x2b) {
    FUN_1008_0517(param_1,param_2,param_3 == 0x79);
    return 1;
  }
  if (param_4 == 0x110) {
    FUN_1008_0c5c();
    uVar4 = param_5;
    CHECKDLGBUTTON(0x1008,DAT_1068_00b6,0x69);
    uVar3 = param_5;
    CHECKDLGBUTTON(0x1160,DAT_1068_2d9c != '\0',0x6d);
    uVar1 = param_5;
    CHECKDLGBUTTON(0x1160,DAT_1068_00bc,0x70);
    FUN_1008_12eb(param_5,DAT_1068_00be,uVar1,uVar3,uVar4);
    return 1;
  }
  if (param_4 != 0x111) {
    return 0;
  }
  if (param_3 == 1) {
    uVar1 = GETFOCUS();
    unaff_CS = 0x1160;
    param_3 = GETDLGCTRLID(0x1160,uVar1);
    if ((param_3 != 0x7d) && (param_3 != 0x7e)) {
      return 0;
    }
  }
  if (param_3 == 0x78) {
    FUN_1008_12eb(param_5,DAT_1068_00be + 0x1e);
    return 1;
  }
  if (param_3 < 0x79) {
    if (param_3 == 0x6d) {
      lVar2 = MAKEPROCINSTANCE(unaff_CS,DAT_1068_2d80,0xa2d);
      if (lVar2 != 0) {
        DIALOGBOX(0x1160,lVar2,0,0xcc,0x1068);
        FREEPROCINSTANCE(0x1160,(int)lVar2);
      }
      CHECKDLGBUTTON(0x1160,DAT_1068_2d9c != '\0',0x6d);
      return 1;
    }
    if (param_3 < 0x6e) {
      if (param_3 == 2) goto LAB_1008_0431;
      if (param_3 != 0x69) {
        return 1;
      }
    }
    else if (param_3 != 0x70) {
      return 1;
    }
    DAT_1068_00b6 = ISDLGBUTTONCHECKED(unaff_CS,0x69);
    DAT_1068_00bc = ISDLGBUTTONCHECKED(0x1160,0x70);
  }
  else {
    if (param_3 == 0x79) {
      FUN_1008_12eb(param_5,DAT_1068_00be + -0x1e);
      return 1;
    }
    if (param_3 == 0x7d) {
      unaff_CS = 0x1008;
      FUN_1008_0dc8();
    }
    else if (param_3 != 0x7e) {
      return 1;
    }
LAB_1008_0431:
    ENDDIALOG(unaff_CS,0);
  }
  return 1;
}


