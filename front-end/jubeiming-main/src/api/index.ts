import axios, {AxiosHeaders} from "axios";
import config from "@/config"

const uriBase = "/api"

const core = {
    me: async(access_token: string) => {
        try {
            const uri = `${uriBase}/me`

            // 定义请求配置
            const headers = new AxiosHeaders({
                "access_token": access_token
            })

            return await axios.get(uri, {
                headers: headers,
                timeout: config.api.timeout,
            })
        } catch (error) {
            return error;
        }
    },
    freshAccessToken: async(refresh_token: string) => {
        try {
            const uri = `${uriBase}/auth/token`

            // 定义请求配置
            const headers = new AxiosHeaders({
                "content-type": "application/json"
            })

            return await axios.post(uri, {
                "grant_type": "refresh_token",
                "refresh_token": refresh_token
            },{
                headers: headers,
                timeout: config.api.timeout,
            })
        } catch (error) {
            return error;
        }
    }
}

config.api.coreUrl

export {core}