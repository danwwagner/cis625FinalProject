for cores in 1 2 4 8 16 
do
qsub -q \*@@elves -l mem=512M,h_rt=03:00:00 -pe single $cores /homes/danwagner/cis625FinalProject/src/ga.sh
qsub -q \*@@elves -l mem=512M,h_rt=03:00:00 -pe single $cores /homes/danwagner/cis625FinalProject/src/ga.sh
qsub -q \*@@elves -l mem=512M,h_rt=03:00:00 -pe single $cores /homes/danwagner/cis625FinalProject/src/ga.sh
qsub -q \*@@elves -l mem=512M,h_rt=03:00:00 -pe single $cores /homes/danwagner/cis625FinalProject/src/ga.sh
qsub -q \*@@elves -l mem=512M,h_rt=03:00:00 -pe single $cores /homes/danwagner/cis625FinalProject/src/ga.sh
qsub -q \*@@elves -l mem=512M,h_rt=03:00:00 -pe single $cores /homes/danwagner/cis625FinalProject/src/ga.sh
qsub -q \*@@elves -l mem=512M,h_rt=03:00:00 -pe single $cores /homes/danwagner/cis625FinalProject/src/ga.sh
qsub -q \*@@elves -l mem=512M,h_rt=03:00:00 -pe single $cores /homes/danwagner/cis625FinalProject/src/ga.sh
qsub -q \*@@elves -l mem=512M,h_rt=03:00:00 -pe single $cores /homes/danwagner/cis625FinalProject/src/ga.sh
qsub -q \*@@elves -l mem=512M,h_rt=03:00:00 -pe single $cores /homes/danwagner/cis625FinalProject/src/ga.sh
done 
