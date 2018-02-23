

.global randomAsm

.data
.text

randomAsm:

addi $v0, $zero, 30        # Syscall 30: System Time syscall
syscall                  # $a0 will contain the 32 LS bits of the system time
add $t0, $zero, $a0     # Save $a0 value in $t0

addi $v0, $zero, 40        # Syscall 40: Random seed
add $a0, $zero, $zero   # Set RNG ID to 0
add $a1, $zero, $t0     # Set Random seed to
syscall

addi $v0, $zero, 42        # Syscall 42: Random int range
add $a0, $zero, $zero   # Set RNG ID to 0
addi $a1, $zero, 4     # Set upper bound to 4 (exclusive)
syscall                  # Generate a random number and put it in $a0
add $s1, $zero, $a0     # Copy the random number to $s1
addi $v0, $s1, 0
