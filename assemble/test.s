movz x1, #0x3F20, lsl #16
movz w2, #0x800, lsl #16
str w2, [x1]

loop:
  movz x3, #0x3F20, lsl #16
  movk x3, #0x001C  
  movz w4 #0x200
  str w4 [x3]

  movz w10, #0xFFFF
 
delayOn:
  subs w10, w10, #0x1
  b.ne delayOn

  movz x5, #0x3F20, lsl #16
  movk x5, #0x0028
  movz w6 #0x200
  str w6 [x5]  

  movz w10, 0xFFFF
 
delayOff:
  subs w10, w10, #0x1
  b.ne delayOff
  
  b loop
