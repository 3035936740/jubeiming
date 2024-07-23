import router from '@/router'
import { ref } from 'vue'

function modifyTitle(title: String) {
  document.title = title
  ref(document.title)
}

export { modifyTitle }
