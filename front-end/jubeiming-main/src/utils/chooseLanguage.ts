function switchLanguage(lang: string) {
  let language: string
  switch (lang.toLowerCase()) {
    case 'zh-cn':
      language = 'zh-cn'
      break
    default:
      language = 'en'
      break
  }
  return language
}

import elem_plus_cn from 'element-plus/es/locale/lang/zh-cn'
import elem_plus_en from 'element-plus/es/locale/lang/en'
function switchLanguageByElementPlus(lang: string) {
  let language: any
  switch (lang.toLowerCase()) {
    case 'zh-cn':
      language = elem_plus_cn
      break
    default:
      language = elem_plus_en
      break
  }
  return language
}

export { switchLanguage, switchLanguageByElementPlus }
