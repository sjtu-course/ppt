FROM micheda/jupyter-opencv:3.4.0
MAINTAINER ben <gaomengping@bytedance>
# Container providing Jupyter notebook server with Python3 bindings for OpenCV 3.4.0
# Based on https://www.pyimagesearch.com/2016/10/24/ubuntu-16-04-how-to-install-opencv/
# Not compiling/installing templates, added gtk support

# install matplotlib and ffmpeg
RUN pip3 install matplotlib==3.0.3
RUN apt-get update && apt-get -y install ffmpeg

# copy resource
COPY image /playground/image
COPY video /playground/video
COPY notebook /playground/notebooks

ENV PYTHONPATH /playground
WORKDIR /playground
CMD ["jupyter", "notebook", "--allow-root", "--ip=0.0.0.0", "--NotebookApp.token=", "/playground"]

EXPOSE 8888/tcp