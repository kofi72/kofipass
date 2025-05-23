# kpass - kofi password manager

(this programm has nothing to do with keepass)

**WARNING:** currently invalid input, where a number is expected, can crash the program discarding all changes.

### features
- encrypted folders
- chacha20 poly1305 (the only supported algorithm)
- custom items
- OTP with libcppotp

### what isn't supported
- attachments
- big chunks of text might be unreadable (can be folded as "hidden text field")
