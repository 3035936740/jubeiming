<script setup lang="ts">
import OrdersItem from '@/components/orders/OrdersItemComponent.vue'
</script>

<template>
  <div class="order-list">
    <div v-if="ordersInfo.length > 0">
      <div v-for="order in ordersInfo">
        <OrdersItem :order-name="order[0]" :order-id="order[1]" :order-status="order[2]"></OrdersItem>
      </div>
    </div>
    <div v-else class="none-order">
      {{ $t('contents.orders.order_tip.none') }}
    </div>
  </div>
</template>

<script lang="ts">
export default {
  name: "OrdersComponent",
  props:{
    ordersInfo: {
      default: []
    }
  }
}
</script>

<style scoped>
.order-list {
  display: flex;
  flex-direction: column;
}

.none-order{
  position: relative;
  text-align: center;
  color: #282828;
  padding: 32px 0;
  font-size: 30px;
  background-color: #fff;
  margin: 20px;
  border-radius: 8px;
}
</style>