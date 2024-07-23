import config from './config.js';
import Client from './client.js';
import fastify from 'fastify';

const app = fastify({ logger: true });

// 定义一个简单的路由
app.get('/', async (request, reply) => {
  return { message: 'phone verify server' };
});

// 定义一个简单的路由
app.post('/send', async (request, reply) => {
  // console.log(await Client.send("19883988084", "456971"))
  const body = request.body
  const phone_number = body['phone_number']
  const verification_code = body['verification_code']
  if(phone_number === undefined || phone_number === null) {
    return reply.status(400).send({
      "status": 400,
      "msg": "phone_number require"
    });
  }

  if(verification_code === undefined || verification_code === null) {
    return reply.status(400).send({
      "status": 400,
      "msg": "verification_code require"
    });
  }

  const authorization = request.headers.authorization

  if (authorization !== config.server.authorization) {
    return reply.status(403).send({
      "status": 403,
      "msg": "Authorization incorrect"
    });
  }

  return await Client.send(phone_number, verification_code);
});

// 启动服务器
const start = async () => {
  try {
    await app.listen({port: config.server.port,  host: config.server.host});
    app.log.info(`Server listening on ${app.server.address().port}`);
  } catch (err) {
    app.log.error(err);
    process.exit(1);
  }
};

start();