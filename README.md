# key2elite

Small tool to send key strokes to the Elite Dangerous game.

Locates the Elite Dangerous client process and sends a set of key strokes. It's give no feedback, except in case of error
in which case it pops a message box. By this, it can easily be used in scripts.

Usage:

key2elite.exe 'Single hardware key scan code as decimal number'

$> key2elite.exe 33

key2elite.exe 'Multiple hardware key scan code as decimal number separated by dash'

$> key2elite.exe 33-33-44-55

Modifiers are added as characters:

$> key2elite.exe 33c-33-44S-55

with c=LCTRL, C=RCTRL, s=LSHIFT, S=RSHIFT, a=LALT, A=RALT
   
Binaries:
https://github.com/imifos/key2elite/releases/tag/1.0

Scan code table: 
https://msdn.microsoft.com/en-us/library/aa299374(v=vs.60).aspx
http://www.philipstorr.id.au/pcbook/book3/scancode.htm
