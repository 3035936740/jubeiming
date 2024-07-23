import { createI18n } from 'vue-i18n'
import { switchLanguage } from '@/utils/chooseLanguage'

const localLang: string = navigator.language

// 定义语言消息的类型
interface MessageSchema {
  [key: string]: any
}

// 动态导入 JSON 文件
const modules = import.meta.glob<{ default: MessageSchema }>('./*.json')

const messages: Record<string, MessageSchema> = {}

// 使用 Promise.all 来等待所有模块加载完成
const loadMessages = async () => {
  const imports = Object.keys(modules).map(async (path) => {
    const lang = path.replace('./', '').replace('.json', '')
    const module = await modules[path]()
    messages[lang] = module.default
  })
  await Promise.all(imports)
}

const chooseLang: string = switchLanguage(localLang)

await loadMessages()

const i18n = createI18n({
  globalInjection: true, // 开启全局多语言渗透
  locale: chooseLang, // 语言环境
  messages, // 语言配置字段
  legacy: false // // 使用 vue3 组合式API 时必须设置为 false
})

export default i18n
