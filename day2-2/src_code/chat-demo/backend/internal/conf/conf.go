package conf

import (
	"flag"
	"fmt"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/viper"
)

var (
	Mode    = flag.String("mode", "dev", "operational mode")
	confDir = flag.String("conf_dir", "conf", "path to config directory")
)

type Conf struct {
	Host      string `json:"host"`
	Port      int    `json:"port"`
	AvatarURL string `json:"group_avatar_url"`
}

func Init() error {
	flag.Parse()
	configPath := fmt.Sprintf("%s/config_%s.toml", *confDir, *Mode)
	log.Infof("使用的配置文件路径: %s\n", configPath)

	viper.SetConfigFile(configPath)
	return viper.ReadInConfig()
}
