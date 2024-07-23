<script setup lang="ts">
import { ref, defineProps } from 'vue'

defineProps<{
  arrow: string
  icon: string
}>()

import useCurrentLanguage from '@/stores/currentSelectLanguage'
import { storeToRefs } from 'pinia'
import { switchLanguage, switchLanguageByElementPlus } from '@/utils/chooseLanguage'
import { useI18n } from 'vue-i18n'

const { locale } = useI18n()

const drawer = ref<boolean>(false)

const currentLanguage = useCurrentLanguage()
const { language } = storeToRefs(currentLanguage)

function updateI18n() {
  locale.value = switchLanguage(language.value)
  switchLanguageByElementPlus(language.value)
}

function selectLanguage(language: string) {
  currentLanguage.switchLanguage(language)
}

const confirm = () => {
  drawer.value = false
  updateI18n()
}

const languages = [
  { key: 'zh-cn', text: '简体中文' },
  { key: 'en', text: 'English' }
]
</script>

<template>
  <div class="item" @click="drawer = true">
    <span>
      <i :class="'fas ' + icon"></i>
      <span style="white-space: nowrap"><slot></slot></span>
    </span>
    <span style="margin: 0 0"></span>

    <i :class="'fas ' + arrow" style="flex-basis: 50px" />
  </div>

  <el-drawer
    direction="btt"
    v-model="drawer"
    title="I am the title"
    :with-header="false"
    size="330px"
    class="custom-drawer"
  >
    <div class="drawer-top">
      <span class="cancel" @click="drawer = false">{{ $t('contents.me.language.cancel') }}</span>
      <span class="confirm" @click="confirm()">{{ $t('contents.me.language.confirm') }}</span>
    </div>
    <span class="select-language">
      <div
        v-for="lang in languages"
        :key="lang.key"
        :class="{ selected: language === lang.key }"
        @click="selectLanguage(lang.key)"
      >
        {{ lang.text }}
      </div>
    </span>
  </el-drawer>
</template>

<style scoped>
.item {
  cursor: pointer;
}

.drawer-top {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 15px;
  margin: 0 20px;
  font-size: 20px;
}

.drawer-top .cancel {
  cursor: pointer;
  color: #9c9c9c;
}

.drawer-top .cancel:active {
  color: #c5c5c5;
}

.drawer-top .confirm {
  cursor: pointer;
  color: #1a8cfd;
}

.drawer-top .confirm:active {
  color: #6db3ff;
}

.select-language {
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  align-content: center;
  height: auto;
  font-size: 24px;
  margin-top: 65px;
}

.select-language div {
  cursor: pointer;
  margin: 5px 0;
  transition:
    transform 0.3s ease,
    font-weight 0.3s ease;
}

.select-language div:hover {
  transform: scale(1.15);
  color: #1a8cfd; /* 设置选中时的颜色 */
}

.select-language div.selected {
  color: #1a8cfd; /* 设置选中时的颜色 */
}
</style>
