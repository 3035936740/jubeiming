<script setup lang="ts">
function explodeDot(event: MouseEvent) {
  const element = event.currentTarget as HTMLElement
  element.classList.add('explode')
  element.addEventListener('animationend', () => {
    element.style.display = 'none' // 爆炸逻辑
  })
}
</script>

<template>
  <div class="notification-dot" @click="explodeDot">
    <slot></slot>
  </div>
</template>

<script lang="ts">
export default {
  name: 'NotificationDotComponent',
  props: {
    dotColor: {
      type: String,
      default: 'red'
    },
    textColor: {
      type: String,
      default: 'white'
    },
    size: {
      type: String,
      default: '22px'
    },
    fontSize: {
      type: String,
      default: '12px'
    }
  }
}
</script>

<style scoped>
.notification-dot {
  cursor: pointer;
  width: v-bind(size);
  height: v-bind(size);
  background-color: v-bind(dotColor);
  color: v-bind(textColor);
  border-radius: 50%;
  padding: 2px;
  font-size: v-bind(fontSize);
  text-align: center;
  font-weight: bold;
  z-index: 100;
}

.notification-dot.explode {
  animation: explode 0.3s forwards;
}

/*爆炸动画*/

@keyframes explode {
  0% {
    transform: scale(1);
    opacity: 1;
  }
  100% {
    transform: scale(2);
    opacity: 0;
  }
}
</style>
