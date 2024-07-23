<script setup lang="ts">
import RecordComponent from '@/components/RecordComponent.vue'
import MessageBarComponent from '@/components/message/MessageBarComponent.vue'
import MessagesChat from '@/components/message/MessagesChatComponent.vue'
import {ref} from "vue";

let list = [
  ["1哥",46],
  ["2哥",7],
  ["3哥",0],
  ["4哥",109],
]

list.forEach((element, key) => {
  if (element[1] > 99) element[1] = '99+'
  else element[1] = element[1].toString()
  list[key] = element
})

const chatList = ref(list)
</script>

<template>
  <div id="message-main">
    <div class="container">
      <MessageBarComponent></MessageBarComponent>
    </div>

    <div class="container" v-if="chatList.length > 0">
      <MessagesChat :chatList="chatList"></MessagesChat>
    </div>
    <div v-else class="none-chat">
      {{ $t('contents.messages.no_records') }}
    </div>
    <RecordComponent></RecordComponent>
  </div>
</template>

<script lang="ts">
export default {
  name: 'MessagesView'
}
</script>

<style scoped>
#message-main {
  position: absolute;
  left: 0;
  right: 0;
}


.none-chat {
  position: relative;
  text-align: center;
  color: #282828;
  padding: 32px 0;
  font-size: 24px;
  background-color: #fff;
  margin: 20px;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

.container {
  background-color: #fff;
  margin: 20px;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}
</style>
