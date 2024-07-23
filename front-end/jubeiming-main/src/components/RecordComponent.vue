<script setup lang="ts"></script>

<template>
  <div class="record">
    <slot>{{ bottomRecord }}</slot>
  </div>
</template>

<script lang="ts">
export default {
  // `setup` 是一个特殊的钩子，专门用于组合式 API。
  data() {
    return {
      bottomRecord: `${this.$t('contents.record.symbol[0]')} ${this.$t('contents.record.year')} ${this.$t('contents.record.company')} | ${this.$t('contents.record.symbol[1]')}: ICP ${this.$t('contents.record.recordNumber')}`
    }
  },
  props: {
    bottom: {
      type: String,
      default: '20%'
    }
  }
}
</script>

<style scoped>
.record {
  position: relative;
  text-align: center;
  color: #aaa;
  padding: 6px 0;
  font-size: 12px;
  margin-bottom: v-bind(bottom);
}
</style>
