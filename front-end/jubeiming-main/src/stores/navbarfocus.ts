import { defineStore } from 'pinia'
import { ref } from 'vue'

const useNavbarFocus = defineStore(
  'navbar-focus',
  () => {
    const elementNumber = ref<number>(0)

    const selectElement = (elementNum: number) => {
      elementNumber.value = elementNum
    }

    return {
      elementNumber,
      selectElement
    }
  },
  {
    persist: true
  }
)

export default useNavbarFocus
