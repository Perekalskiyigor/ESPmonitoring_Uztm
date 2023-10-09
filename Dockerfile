# Use multi-stage build for Python and Go
FROM python:3.11-slim AS python-builder

# Copy the application to the /app directory
COPY / /app

# Set the working directory
WORKDIR /app

# Copy the requirements.txt file to the /app directory
COPY requirements.txt /app

# Install the Python dependencies
RUN pip3 install -r /app/requirements.txt --no-cache-dir



FROM golang:1.16 AS go-builder
WORKDIR /app

# Install ca-certificates and update the certificates
RUN apt-get update && apt-get install -y ca-certificates openssl

COPY go.mod /app

# Update the certificates
RUN update-ca-certificates

COPY main.go /app


# Add the missing package to your go.mod file
RUN go get -u github.com/denisenkom/go-mssqldb
# Ensure your go.mod file is in the correct location
#RUN mv app/go.mod /app
# Ensure your main.go file is in the correct location
#RUN mv app/main.go /app
RUN go build -o server main.go

CMD ["/bin/bash", "-c", "python3 bot.py && /app/server &"]

