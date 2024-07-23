<script setup lang="ts">
import useSelectOrderId from '@/stores/selectOrderId'
import {storeToRefs} from "pinia";
const SelectOrderId = useSelectOrderId()
const { show_drawer } = storeToRefs(SelectOrderId)
</script>

<template>
  <div class="order-item">
    <div class="order-info">
      <img :src="src" alt="order picture">
      <div>
        <p>{{ $t('contents.orders.orders_name') }}: {{ orderName }}</p>
        <p>{{ $t('contents.orders.orders_id') }}: {{ orderId }}</p>
      </div>
    </div>
    <div class="order-status" :class="buttonColorChange">{{ $t('contents.orders.status[' + orderStatus + ']') }}</div>
    <div class="order-actions">
      <button @click="SelectOrderId.setOrderID(orderId);show_drawer = true;">{{ $t('contents.orders.detail') }}</button>
    </div>
  </div>
</template>

<script lang="ts">
export default {
  name: "OrdersItemComponent",
  props: {
    src: {
      type: String,
      default: "https://via.placeholder.com/40"
    },
    orderName: {
      type: String,
      require: true
    },
    orderId: {
      type: Number,
      require: true
    },
    orderStatus: {
      type: Number,
      default: 0
    }
  },
  data() {
    return {
      buttonColorChange: {
        'order-status-color-0': this.orderStatus === 0,
        'order-status-color-1': this.orderStatus === 1,
        'order-status-color-2': this.orderStatus === 2,
        'order-status-color-3': this.orderStatus === 3
      }
    }
  }
}



</script>

<style scoped>
.order-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 10px;
  border-bottom: 1px solid #eee;
}

.order-item:last-child {
  border-bottom: none;
}

.order-info {
  display: flex;
  align-items: center;
}

.order-info img {
  width: 40px;
  height: 40px;
  margin-right: 10px;
  border-radius: 5px;
}

.order-status{
  font-weight: bold;
}

.order-status-color-0{
  color: #696c69;
}

.order-status-color-1{
  color: #0645c7;
}

.order-status-color-2{
  color: #FF0000FF;
}

.order-status-color-3{
  color: #06c406;
}

.order-actions button {
  background-color: #007bff;
  color: white;
  border: none;
  padding: 5px 10px;
  border-radius: 5px;
  cursor: pointer;
}
</style>