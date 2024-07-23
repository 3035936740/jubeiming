// config.ts

// 定义接口描述配置对象的结构
interface AppConfig {
    api: {
        timeout: number
        coreUrl: string
    }
}

// 创建配置对象
const config: AppConfig = {
    api: {
        timeout: 15000,
        coreUrl: "http://127.0.0.1:38765"
    }
};

// 导出配置对象
export default config;