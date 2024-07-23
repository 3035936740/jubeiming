<script lang="ts" setup></script>

<template>
  <RouterLink :to="to" class="nav-item">
    <div class="item">
      <span>
        <slot></slot>
        <span>{{ content }}</span>
      </span>
      <i :class="'fas ' + arrow" style="flex-basis: 50px" />
    </div>
  </RouterLink>
</template>

<script lang="ts">
export default {
  name: 'PersonalOtherComponent',
  props: {
    to: {
      type: String,
      default: '#'
    },
    arrow: {
      type: String,
      default: 'fa-chevron-right'
    },
    content: {
      type: String,
      default: ''
    }
  }
}
</script>

<style scoped>
.nav-item {
  display: contents;
  text-decoration: none;
  color: black;
}
</style>
