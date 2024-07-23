<script setup lang="ts">
import UserComponent from '@/components/me/UserComponent.vue'
import RecordComponent from '@/components/RecordComponent.vue'
import SelectLang from '@/components/me/SelectLangComponent.vue'
import PersonalOtherComponent from '@/components/me/PersonalOtherComponent.vue'
import LogoutComponent from '@/components/me/LogoutComponent.vue'
import { ref } from 'vue'

const avatar = ref('/jubeiming.png')
const arrow = ref('fa-chevron-right')
</script>

<template>
  <div id="me-main">
    <div class="container">
      <UserComponent :src="avatar">{{ $t('contents.name') }}</UserComponent>
      <SelectLang icon="fa-language" :arrow="arrow">{{
        $t('contents.me.language.content_name')
      }}</SelectLang>
      <PersonalOtherComponent to="#" :content="$t('contents.me.guide.content_name')" :arrow="arrow"
        ><i class="fas fa-book"></i
      ></PersonalOtherComponent>
      <PersonalOtherComponent
        to="#"
        :content="$t('contents.me.statistics.content_name')"
        :arrow="arrow"
        ><i class="fas fa-chart-bar"></i
      ></PersonalOtherComponent>
      <PersonalOtherComponent
        to="#"
        :content="$t('contents.me.distribution.content_name')"
        :arrow="arrow"
        ><i class="fas fa-coins"></i
      ></PersonalOtherComponent>
      <PersonalOtherComponent
        to="#"
        :content="$t('contents.me.setting.content_name')"
        :arrow="arrow"
        ><i class="fas fa-cog"></i
      ></PersonalOtherComponent>
      <LogoutComponent :content="$t('contents.me.logout')" v-show="true"
        ><i class="fas fa-sign-out"
      /></LogoutComponent>
    </div>

    <RecordComponent bottom="20%"></RecordComponent>
  </div>
</template>
<style>
SelectLang {
  position: relative;
  margin: 5px 5px;
}

#me-main {
  position: absolute;
  left: 0;
  right: 0;
}

.container {
  background-color: #fff;
  margin: 20px;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

.item {
  display: flex;
  align-content: flex-start;
  justify-content: space-between;
  align-items: center;
  padding: 15px;
  border-bottom: 1px solid #e0e0e0;
}

.item:hover {
  background-color: #cccccc;
}

.item:active {
  background-color: #a0a0a0;
}

.item i {
  width: 24px;
  height: 24px;
  margin-right: 10px;
  font-size: 24px;
}

.item span {
  margin: 0px 6px;
  flex-grow: 1;
}
</style>
