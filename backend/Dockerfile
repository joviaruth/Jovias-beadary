FROM alpine:3.20 AS build
RUN apk add --no-cache build-base cmake git asio-dev
WORKDIR /app
COPY . .
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel

FROM alpine:3.20
RUN apk add --no-cache libstdc++
WORKDIR /app
COPY --from=build /app/build/jovias-api .
ENV PORT=8080
EXPOSE 8080
CMD ["./jovias-api"]
