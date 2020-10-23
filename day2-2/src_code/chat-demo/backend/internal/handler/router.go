package handler

import (
	"fmt"
	"net/http"

	"github.com/gin-gonic/gin"
	gosocketio "github.com/graarh/golang-socketio"
	"github.com/spf13/viper"
)

func Run(server *gosocketio.Server) error {
	router := gin.Default()
	router.Use(Cors())

	registerRouter(router, server)

	host := viper.GetString("address.host")
	port := viper.GetInt("address.port")
	address := fmt.Sprintf("%s:%d", host, port)

	return router.Run(address)
}

func registerRouter(r *gin.Engine, s *gosocketio.Server) {
	r.GET("/socket.io/*any", gin.WrapH(s))
	r.POST("/socket.io/*any", gin.WrapH(s))

	r.GET("/health-check", func(c *gin.Context) {
		c.AbortWithStatusJSON(http.StatusOK, gin.H{"code": 0, "msg": "healthy"})
	})
}
