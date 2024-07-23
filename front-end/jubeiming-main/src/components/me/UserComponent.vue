<script setup lang="ts">
import { defineProps } from 'vue'

const props = defineProps<{
  src: string
}>()
</script>

<template>
  <div class="profile">
    <img :src="props.src" alt="avatar" />
    <span><slot>loading......</slot></span>
  </div>
</template>

<script lang="ts">
export default {
  name: 'UserComponent'
}
</script>

<style scoped>
.profile {
  display: flex;
  align-items: center;
  padding: 15px;
  border-bottom: 1px solid #e0e0e0;
}

.profile img {
  width: 50px;
  height: 50px;
  border-radius: 50%;
  margin-right: 15px;
}

.profile span {
  text-overflow: ellipsis;
  overflow: hidden;
  white-space: nowrap;
  font-size: 18px;
  font-weight: bold;
}
</style>
