.PHONY: duo

duo:
	particle compile duo duo
	particle flash --serial duo_firmware_*
	rm duo_firmware_*

scratch/trainer: car/Dockerfile
	docker build --tag kong-trainer car
	touch scratch/trainer

models/sample: scratch/trainer tubs/sample
	docker run -it \
		-v "`pwd`/tubs/sample:/app/tub" \
		-v "`pwd`/models:/app/models" \
		kong-trainer train --tub /app/tub --model /app/models/sample
