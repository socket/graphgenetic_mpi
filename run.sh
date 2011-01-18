rm *.run.o*
rm *.run.e*
qsub -l nodes=2:ppn=4 -d ./ gg.run

