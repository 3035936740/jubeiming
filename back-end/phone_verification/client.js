// client.js
import config from './config.js';
import OpenApi, {Config} from '@alicloud/openapi-client';
import OpenApiUtil from '@alicloud/openapi-util';
import Util from '@alicloud/tea-util';

class Client {

  /**
   * 使用AK&SK初始化账号Client
   * @return Client
   * @throws Exception
   */
  static createClient() {
    let client_config = new Config({
      // 必填，请确保代码运行环境设置了环境变量 ALIBABA_CLOUD_ACCESS_KEY_ID。
      accessKeyId: config.AccessKeyId,
      // 必填，请确保代码运行环境设置了环境变量 ALIBABA_CLOUD_ACCESS_KEY_SECRET。
      accessKeySecret: config.AccessKeySecret,
    });
    // Endpoint 请参考 https://api.aliyun.com/product/Dysmsapi
    client_config.endpoint = config.endpoint;

    return new OpenApi.default(client_config);
  }

  /**
   * API 相关
   * @param path params
   * @return OpenApi.Params
   */
  static createApiInfo() {
    let params = new OpenApi.Params({
      // 接口名称
      action: 'SendSms',
      // 接口版本
      version: '2017-05-25',
      // 接口协议
      protocol: 'HTTPS',
      // 接口 HTTP 方法
      method: 'POST',
      authType: 'AK',
      style: 'RPC',
      // 接口 PATH
      pathname: `/`,
      // 接口请求体内容格式
      reqBodyType: 'json',
      // 接口响应体内容格式
      bodyType: 'json',
    });
    return params;
  }

  static async send(phone_numbers, code) {
    let client = Client.createClient();
    let params = Client.createApiInfo();
    // query params
    let queries = { };
    queries['PhoneNumbers'] = `${phone_numbers}`;
    queries['SignName'] = config.SignName;
    queries['TemplateCode'] = config.TemplateCode;
    queries['TemplateParam'] = `{"code":"${code}"}`;
    // runtime options
    let runtime = new Util.RuntimeOptions({ });
    let request = new OpenApi.OpenApiRequest({
      query: OpenApiUtil.default.query(queries),
    });

    // 复制代码运行请自行打印 API 的返回值
    // 返回值为 Map 类型，可从 Map 中获得三类数据：响应体 body、响应头 headers、HTTP 返回的状态码 statusCode。
    let resp = await client.callApi(params, request, runtime);
    return Util.default.toJSONString(resp);
  }

}

export default Client