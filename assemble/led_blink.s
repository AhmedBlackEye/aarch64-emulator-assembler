movz x1, #0x3f20, lsl #16
movz x2, #0x800, lsl #16
str w2, [x1]

loop:
  movz x3, #0x3f20, lsl #16
  movk x3, #0x001c  
  movz x4, #0x200
  str w4, [x3]

  movz x10, #0x10, lsl #16 
 
delayOn:
  subs x10, x10, #0x1
  b.ne delayOn

  movz x5, #0x3f20, lsl #16
  movk x5, #0x0028
  movz x6, #0x200
  str w6, [x5] 

  movz x10, #0x10, lsl #16 
 
delayOff:
  subs x10, x10, #0x1
  b.ne delayOff
  
  b loop

	