<script setup lang="ts">
import MessageBarItem from '@/components/message/MessageBarItemComponent.vue'
import NotificationDotComponent from '@/components/message/NotificationDotComponent.vue'
import { ref } from 'vue'

let quantity: Number[] | String[] = [100, 27, 0, 3]

quantity.forEach((element: Number | String, key) => {
  if (element > 99) element = '99+'
  else element = element.toString()
  quantity[key] = element
})

let notificationDotQuantity = ref(quantity)
</script>

<template>
  <div class="icon-bar">
    <MessageBarItem :title="$t('contents.messages.notifications.like')" icon="fa-thumbs-up">
      <NotificationDotComponent
        class="notification-dot"
        v-show="notificationDotQuantity[0] !== '0'"
        >{{ notificationDotQuantity[0] }}</NotificationDotComponent
      >
    </MessageBarItem>
    <MessageBarItem :title="$t('contents.messages.notifications.reply')" icon="fa-comment-dots">
      <NotificationDotComponent
        class="notification-dot"
        v-show="notificationDotQuantity[1] !== '0'"
        >{{ notificationDotQuantity[1] }}</NotificationDotComponent
      >
    </MessageBarItem>
    <MessageBarItem :title="$t('contents.messages.notifications.at_me')" icon="fa-at">
      <NotificationDotComponent
        class="notification-dot"
        v-show="notificationDotQuantity[2] !== '0'"
        >{{ notificationDotQuantity[2] }}</NotificationDotComponent
      >
    </MessageBarItem>
    <MessageBarItem :title="$t('contents.messages.notifications.notification')" icon="fas fa-bell">
      <NotificationDotComponent
        class="notification-dot"
        v-show="notificationDotQuantity[3] !== '0'"
        >{{ notificationDotQuantity[3] }}</NotificationDotComponent
      >
    </MessageBarItem>
  </div>
</template>

<script lang="ts">
export default {
  name: 'MessageBarComponent'
}
</script>

<style scoped>
.icon-bar {
  display: flex;
  align-items: flex-start;
  justify-content: space-around;
  padding: 20px;
  border-bottom: 1px solid #ddd;
}

.notification-dot {
  position: absolute;
  top: -11px;
  right: -21px;
}
</style>
