for i in {1..5};
do
	./check ./output/TIME_MEASUREMENT_dmesg.txt OS_PJ1_Test/FIFO_$i.txt output/FIFO_$i\_stdout.txt  output/FIFO_$i\_dmesg.txt 
done

for i in {1..5};
do
	./check ./output/TIME_MEASUREMENT_dmesg.txt OS_PJ1_Test/RR_$i.txt output/RR_$i\_stdout.txt  output/RR_$i\_dmesg.txt 
done

for i in {1..5};
do
	./check ./output/TIME_MEASUREMENT_dmesg.txt OS_PJ1_Test/SJF_$i.txt output/SJF_$i\_stdout.txt  output/SJF_$i\_dmesg.txt 
done

for i in {1..5};
do
	./check ./output/TIME_MEASUREMENT_dmesg.txt OS_PJ1_Test/PSJF_$i.txt output/PSJF_$i\_stdout.txt  output/PSJF_$i\_dmesg.txt 
done
