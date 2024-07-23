<script setup lang="ts">
import { RouterLink, RouterView } from 'vue-router'
import NavbarView from '@/views/NavbarView.vue'
import useCurrentLanguage from '@/stores/currentSelectLanguage'
import { storeToRefs } from 'pinia'
import { switchLanguage } from '@/utils/chooseLanguage'
import { useI18n } from 'vue-i18n'

const currentLanguage = useCurrentLanguage()
const { language } = storeToRefs(currentLanguage)
const { locale } = useI18n()
locale.value = switchLanguage(language.value)
/*
import switchLang from "@/utils/switchLang";

switchLang("en")
*/
</script>

<template>
  <RouterView></RouterView>
  <NavbarView></NavbarView>
</template>

<script lang="ts">

export default {
  name: 'App',
  setup() {
    return {}
  },
  computed: {
  },
  async mounted() {
  },

  // const p = await core.me("owrxw773ekg4rfy5");
  // console.log(p)
}
</script>

<style scoped>
RouterView {
}
</style>
