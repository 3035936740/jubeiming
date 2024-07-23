<script setup lang="ts">
import ProfileItem from '@/components/message/ProfileItemComponent.vue'
import NotificationDotComponent from '@/components/message/NotificationDotComponent.vue'

</script>

<template>
    <div v-for="item in chatList">
      <ProfileItem class="profile-item" :username="item[0]"><NotificationDotComponent v-if="item[1]!=='0'">{{ item[1] }}</NotificationDotComponent></ProfileItem>
    </div>
</template>

<script lang="ts">
export default {
  name: "MessagesChatComponent",
  props: {
    chatList: {
      default: []
    }
  }
}
</script>

<style scoped>
.profile-item:hover{
  background-color: #cccccc;
}

.profile-item:active{
  background-color: #a0a0a0;
}
</style>