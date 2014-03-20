 set srcdir=~csci531/public/cs531/hw5
#set srcdir=/home/feng/WorkPlace/CSCI531HW/HW5/DES/files
#(A) tablecheck (+20 points)
#	1. good tablefile (+10 points, +1 points for each correct case)

		foreach f (0 1 2 3 4 5 6 7 8 9)
                        echo "==> $srcdir/f$f"
			./hw5 tablecheck -t=$srcdir/f$f
		end

#	2. bad tablefile (+10 points, +1 points for each correct case)
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

#(B) encrypt/decrypt (+48 points)

#	1. standard DES testvector (+8 points, +1 for each correct case)
                set key=0123456789abcdef
                /bin/rm -f fout ferr
                #
                # the first 8 bytes of the standard DES testvector is in f20
                #
                (./hw5 encrypt -k=$key -t=$srcdir/des.in $srcdir/f20 > fout) >& ferr
                echo "==> $srcdir/f20.ct"
                diff fout $srcdir/f20.ct
                echo "==> $srcdir/f20.1.err"
                diff ferr $srcdir/f20.1.err
                /bin/rm -f fout ferr
                (./hw5 decrypt -k=$key -t=$srcdir/des.in $srcdir/f20.ct > fout) >& ferr
                echo "==> $srcdir/f20"
                diff fout $srcdir/f20
                echo "==> $srcdir/f20.2.err"
                diff ferr $srcdir/f20.2.err
                /bin/rm -f fout ferr

                #
                # the standard DES testvector is in f21
                #
                (./hw5 encrypt -k=$key -t=$srcdir/des.in $srcdir/f21 > fout) >& ferr
                echo "==> $srcdir/f21.ct"
                diff fout $srcdir/f21.ct
                echo "==> $srcdir/f21.1.err"
                diff ferr $srcdir/f21.1.err
                /bin/rm -f fout ferr
                (./hw5 decrypt -k=$key -t=$srcdir/des.in $srcdir/f21.ct > fout) >& ferr
                echo "==> $srcdir/f21"
                diff fout $srcdir/f21
                echo "==> $srcdir/f21.2.err"
                diff ferr $srcdir/f21.2.err
                /bin/rm -f fout ferr

#	2. encrypt (+10 points, +1 for each correct answer)
                set key=0123456789abcdef
                /bin/rm -f fout.? ferr.?
                #
                # use various tablefile to encrypt f22
                #
				foreach f (0 1 2 3 4 5 6 7 8 9)
                        (./hw5 encrypt -k=$key -t=$srcdir/f$f $srcdir/f22 > fout.$f) >& ferr.$f
                        echo "==> $srcdir/f22.$f.enc"
                        diff fout.$f $srcdir/f22.$f.enc
                        echo "==> $srcdir/f22.$f.err"
                        diff ferr.$f $srcdir/f22.$f.err
                end
                /bin/rm -f fout.? ferr.?

#	3. encrypt from stdin (+10 points, +1 for each correct answer)
                set key=0123456789abcdef
                /bin/rm -f fout.? ferr.?
                #
                # use various tablefile to encrypt f23 from stdin
                #
				foreach f (0 1 2 3 4 5 6 7 8 9)
                        cat $srcdir/f23 | (./hw5 encrypt -k=$key -t=$srcdir/f$f > fout.$f) >& ferr.$f
                        echo "==> $srcdir/f23.$f.enc"
                        diff fout.$f $srcdir/f23.$f.enc
                        echo "==> $srcdir/f23.$f.err"
                        diff ferr.$f $srcdir/f23.$f.err
                end
                /bin/rm -f fout.? ferr.?

#	4. decrypt (+10 points, +1 for each correct answer)
                set key=0123456789abcdef
                /bin/rm -f fout.? ferr.?
                #
                # use various tablefile to decrypt
                #
				foreach f (0 1 2 3 4 5 6 7 8 9)
                        (./hw5 decrypt -k=$key -t=$srcdir/f$f $srcdir/f22.$f.enc > fout.$f) >& ferr.$f
                        echo "==> fout.$f"
                        diff fout.$f $srcdir/f24
                        echo "==> $srcdir/f24.$f.err"
                        diff ferr.$f $srcdir/f24.$f.err
                end
                /bin/rm -f fout.? ferr.?

#	5. decrypt from stdin (+10 points, +1 for each correct answer)
                set key=0123456789abcdef
                /bin/rm -f fout.? ferr.?
                #
                # use various tablefile to decrypt from stdin
                #
				foreach f (0 1 2 3 4 5 6 7 8 9)
                        cat $srcdir/f22.$f.enc | (./hw5 decrypt -k=$key -t=$srcdir/f$f > fout.$f) >& ferr.$f
                        echo "==> fout.$f"
                        diff fout.$f $srcdir/f24
                        echo "==> $srcdir/f24.$f.err"
                        diff ferr.$f $srcdir/f24.$f.err
                end
                /bin/rm -f fout.? ferr.?

#(C) encrypt, then decrypt using student's code (+10 points, +1 for each correct answer)
                set key=0123456789abcdef
                /bin/rm -f fout fout.?
                #
                # use various tablefile to encrypt f22 then decrypt
                # the resulting output should be the same as f24
                # ignore all data that goes to stderr
                #
                foreach f (0 1 2 3 4)
                        /bin/rm -f fout
                        (./hw5 encrypt -k=$key -t=$srcdir/f$f $srcdir/f22 > fout) >& /dev/null
                        (./hw5 decrypt -k=$key -t=$srcdir/f$f fout > fout.$f) >& /dev/null
                        echo "==> fout.$f"
                        diff fout.$f $srcdir/f24
                end
                /bin/rm -f fout fout.?

                #
                # use various tablefile to encrypt f23 then decrypt
                # the resulting output should be the same as f25
                # ignore all data that goes to stderr
                #
                foreach f (5 6 7 8 9)
                        /bin/rm -f fout
                        (./hw5 encrypt -k=$key -t=$srcdir/f$f $srcdir/f23 > fout) >& /dev/null
                        (./hw5 decrypt -k=$key -t=$srcdir/f$f fout > fout.$f) >& /dev/null
                        echo "==> fout.$f"
                        diff fout.$f $srcdir/f25
                end
                /bin/rm -f fout fout.?

#(D) encrypt3/decrypt3 (+20 points)

#        1. encrypt3 (+10 points, +1 for each correct answer)
                set key3old=c78a1fab0218773dea4a2e65e999e2557637d934f0841173
                set key3=1596369d5aa562355453ae44101c2614c729a00607319591
                /bin/rm -f fout.?.enc
                #
                # use various tablefile to 3des encrypt f32
                #
				foreach f (0 1 2 3 4 5 6 7 8 9)
                        (./hw5 encrypt3 -k=$key3 -t=$srcdir/f$f $srcdir/f32 > fout.$f.enc) >& /dev/null
                        echo "==> $srcdir/f32.$f.enc"
                        diff fout.$f.enc $srcdir/f32.$f.enc
                end
                /bin/rm -f fout.?.enc

#		2. decrypt3 (+10 points, +1 for each correct answer)
                set key3old=c78a1fab0218773dea4a2e65e999e2557637d934f0841173
                set key3=1596369d5aa562355453ae44101c2614c729a00607319591
                /bin/rm -f fout.?.dat
                #
                # use various tablefile to 3des decrypt
                #
				foreach f (0 1 2 3 4 5 6 7 8 9)
                        (./hw5 decrypt3 -k=$key3 -t=$srcdir/f$f $srcdir/f32.$f.enc > fout.$f.dat) >& /dev/null
                        echo "==> fout.$f.dat"
                        diff fout.$f.dat $srcdir/f34
                end
                /bin/rm -f fout.?.dat
                
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
