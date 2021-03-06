#set srcdir=~csci531/public/cs531/hw5
set srcdir=/home/feng/WorkPlace/CSCI531HW/HW5/DES/files
		foreach f (0 1 2 3 4 5 6 7 8 9)
            echo "==> $srcdir/f$f"
			./hw5 tablecheck -t=$srcdir/f$f
		end

                #
                # each should produce a reasonable error message
                # here are what can be reported (may be reported in other manners):
                #   f10: (PC2) the value 30 in the PC2 table occur too many times
                #   f11: (PC1) the value 21 in the PC1 table did not occur
                #   f12: (  V) the value 1 in the V table did not occur 4 times
                #   f13: (  V) the value 2 in the V table did not occur 12 times
                #   f14: ( S8) 63 values read while 64 was expected
                #   f15: ( S4) the value 10 in the S4 table did not occur 4 times
                #   f16: ( S1) the value 11 in the S1 table did not occur 4 times
                #   f17: (  P) 34 values read while 32 was expected
                #   f18: (  E) the value 16 in the E table occur too many times
                #   f19: ( IP) too few values read
                #

		foreach f (10 11 12 13 14 15 16 17 18 19)
            echo "==> $srcdir/f$f"
			./hw5 tablecheck -t=$srcdir/f$f
		end
                ./hw5
                   # (malformed command)
                ./hw5 stream -out=foo 
                   # (malformed command)
                ./hw5 keygen -x="passphrase"
                   # (malformed command)
                ./hw5 tablecheck -t=/etc/motd
                   # (malformed tablefile)
                ./hw5 tablecheck -t=/etc/foobar1
                   # (cannot open input file)
                ./hw5 encrypt -t=
                   # (malformed command)
                ./hw5 decrypt
                   # (malformed command)
                ./hw5 encrypt -k=0123456789abcdef -t=$srcdir/des.in /etc/fb1
                   # (cannot open input file)
                ./hw5 encrypt -k=/home/scf-22/csci531/.login -t=$srcdir/des.in
                   # (malformed key)
                ./hw5 encrypt -k=817236_723647562 -t=$srcdir/des.in /etc/passwd
                   # (malformed key)
                ./hw5 decrypt -k=0123456789abcdef -t=/etc/passwd
                   # (malformed tablefile)
                ./hw5 encrypt3 -k=8172362723647562 -t=$srcdir/des.in /etc/passwd
                   # (key too short)
                ./hw5 decrypt3 -k=0123456789abcdef0123456789abcdef0123456789abcdef -t=/etc/motd
                   # (malformed tablefile)
