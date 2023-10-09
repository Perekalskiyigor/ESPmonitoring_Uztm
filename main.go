package main

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"time"

	_ "github.com/denisenkom/go-mssqldb"
)

const connectionString = "sqlserver://Integration_User:w6RGG2Wy16EIr4F@extDB.uralmash.ru?database=BI_WORK_REP&connection+timeout=3000&dial+timeout=3000"

type ESPData struct {
	Sensor1 float64 `json:"sensor1"`
	Sensor2 float64 `json:"sensor2"`
	Sensor3 float64 `json:"sensor3"`
	Sensor4 float64 `json:"sensor4"`
	Sensor5 float64 `json:"sensor5"`
	Sensor6 float64 `json:"sensor6"`
	IP      string  `json:"ip"`
}

func main() {
	go startServer()

	/*
	// Run the bot script
	cmd := exec.Command("python", "bot.py")
	err := cmd.Run()
	if err != nil {
		log.Fatal(err)
	}
	*/

	// Schedule the execution of handleESPData every 5 seconds
	ticker := time.NewTicker(300 * time.Second)
	for {
		select {
		case <-ticker.C:
			go handleESPData()
		}
	}

	
}

func startServer() {
	//http.HandleFunc("/", handleESPData)

	// Start the HTTP server on port 80
	err := http.ListenAndServe(":80", nil)
	if err != nil {
		log.Fatal(err)
	}
}

func handleESPData() {
	response, err := http.Get("http://10.11.8.207/")
	if err != nil {
		log.Println("Error making HTTP request:", err)
		return
	}
	defer response.Body.Close()

	// Read the response body
	body, err := ioutil.ReadAll(response.Body)
	if err != nil {
		log.Println("Error reading response body:", err)
		return
	}

	// Decode the JSON response
	var data ESPData
	err = json.Unmarshal(body, &data)
	if err != nil {
		log.Println("Error decoding JSON response:", err)
		return
	}

	// Insert the data into the database
	db, err := sql.Open("sqlserver", connectionString)
	if err != nil {
		log.Println("Error opening database connection:", err)
		return
	}
	defer db.Close()

	_, err = db.Exec("[BI_TEST].[dbo].[ESPReceiver]",
		sql.Named("IP", data.IP),
		sql.Named("Sensor1", data.Sensor1),
		sql.Named("Sensor2", data.Sensor2),
		sql.Named("Sensor3", data.Sensor3),
		sql.Named("Sensor4", data.Sensor4),
		sql.Named("Sensor5", data.Sensor5),
		//sql.Named("Sensor6", data.Sensor6),
	)

	if err != nil {
		log.Println("Error inserting data into the database:", err)
		return
	}

	fmt.Println("Connected to the database successfully!")
}


