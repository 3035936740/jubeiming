<script lang="ts" setup></script>

<template>
  <div class="item" v-show="true">
    <span>
      <slot></slot>
      <span>{{ content }}</span>
    </span>
  </div>
</template>

<script lang="ts">
export default {
  name: 'LogoutComponent',
  props: {
    content: {
      type: String,
      default: ''
    }
  }
}
</script>

<style scoped>
.item {
  cursor: pointer;
}
</style>
