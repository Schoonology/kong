FROM donkeycar

RUN pip3 install tensorflow
RUN pip3 install pyserial
ADD ./parts /app/parts
ADD ./config.py /app/config.py
ADD ./manage.py /app/manage.py

ENTRYPOINT ["python3", "manage.py"]
