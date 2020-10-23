FROM node:latest
WORKDIR /app
COPY . .
RUN npm install && \
    npm run build && \
    npm install -g serve
EXPOSE 3000
ENTRYPOINT ["serve", "-s", "dist", "-l", "tcp:0.0.0.0:3000"]
