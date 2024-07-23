<script setup lang="ts"></script>

<template>
  <div class="dev-main">
    <div class="dev-template">
      <i class="fas fa-digging" />
      <span style="margin-left: 25px">{{ $t('contents.dev.dev_page') }}</span>
    </div>
  </div>
</template>

<script lang="ts">
export default {
  name: 'TestView.vue'
}
</script>

<style scoped>
.dev-main {
  position: absolute;
  height: 100%;
  left: 0;
  right: 0;
}

.dev-template {
  margin: 38px;
  display: flex;
  flex-direction: row;
  justify-content: center;
  align-content: center;
  font-size: 36px;
}
</style>
