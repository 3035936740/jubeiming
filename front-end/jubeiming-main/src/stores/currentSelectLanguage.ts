import { defineStore } from 'pinia'
import { ref } from 'vue'

const useCurrentLanguage = defineStore(
  'current-select-language',
  () => {
    const language = ref<string>(navigator.language.toLowerCase())

    const switchLanguage = (languageType: string) => {
      language.value = languageType
    }

    return {
      language,
      switchLanguage
    }
  },
  {
    persist: true
  }
)

export default useCurrentLanguage
