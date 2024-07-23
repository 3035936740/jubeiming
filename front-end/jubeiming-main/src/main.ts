import './assets/main.css'

import i18n from './i18n'
import { createApp } from 'vue'
import { createPinia } from 'pinia'
import piniaPluginPersistedstate from 'pinia-plugin-persistedstate'
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'
import * as Icons from '@element-plus/icons-vue'

import App from './App.vue'
import router from './router'
import { switchLanguage, switchLanguageByElementPlus } from './utils/chooseLanguage'

const app = createApp(App)
import init from './init'

import '@fortawesome/fontawesome-free/css/all.css'
import '@fortawesome/fontawesome-free/js/all'

// 注册所有 Element Plus 图标组件
for (const [key, component] of Object.entries(Icons)) {
  app.component(key, component)
}

const pinia = createPinia()
pinia.use(piniaPluginPersistedstate)
app
  .use(pinia)
  .use(router)
  .use(i18n)
  .use(ElementPlus, {
    locale: switchLanguageByElementPlus(switchLanguage(navigator.language))
  })

init()

app.mount('#app')
