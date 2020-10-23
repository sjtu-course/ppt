package main

import (
	"chat-demo-backend/internal/conf"
	"chat-demo-backend/internal/handler"
	"fmt"
	"io"
	"os"

	"github.com/gin-gonic/gin"
	log "github.com/sirupsen/logrus"
	"github.com/spf13/viper"
)

func init() {
	if err := conf.Init(); err != nil {
		panic(err)
	}

	if err := initLogger(); err != nil {
		panic(err)
	}
}

func main() {
	server := handler.InitSocketIOServer()
	log.Fatal(handler.Run(server))
}

func initLogger() error {
	log.SetReportCaller(true)

	// redirect access log
	path := viper.GetString("log.access")
	file, err := os.Create(path)
	if err != nil {
		return fmt.Errorf("fail to create access log file, path=%s, err_msg=%w", path, err)
	}
	gin.DefaultWriter = io.MultiWriter(file)

	// redirect app log
	if *conf.Mode == "prod" {
		log.SetLevel(log.InfoLevel)
	}
	path = viper.GetString("log.app")
	file, err = os.Create(path)
	if err != nil {
		return fmt.Errorf("fail to create app log file, path=%s, err_msg=%w", path, err)

	}
	log.SetOutput(file)

	return nil
}
