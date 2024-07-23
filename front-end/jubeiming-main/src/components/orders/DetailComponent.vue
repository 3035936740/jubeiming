<template>
  <div class="detail-box">
    <div class="order-detail-image-item">
      <img src="https://via.placeholder.com/80" width="80px" height="80px" />
      <div class="order-sub-box">
        <div class="order-name">
          我也不知道叫什么(wwwwwwwwwwwwwwwwwwww
        </div>
        <div class="order-price">
          <span style="font-size: 16px">¥</span><span class="price">257.59</span>
        </div>
      </div>
    </div>
    <div class="list-box">
      <div class="price-calc">
        <span>单价</span>
        <span><span style="font-size: 16px">¥</span><span class="price">257.59</span></span>
      </div>
      <div class="price-calc">
        <span>数量</span>
        <span>2</span>
      </div>
      <div class="price-calc">
        <span>折扣</span>
        <span style="color:green;"><span style="font-size: 16px">¥</span><span class="price">15.0</span></span>
      </div>
      <div class="price-calc">
        <span>总价</span>
        <span style="color: crimson"><span style="font-size: 16px">¥</span><span class="price">500.18</span></span>
      </div>
    </div>
    <hr style="color:grey">

    <div class="list-box" style="padding-bottom: 0px">
      <div class="order-other">
        <span>订单编号</span>
        <span>1145141919810</span>
      </div>
      <div class="order-other">
        <span>收货地址</span>
        <span>我不知道(</span>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: "DetailComponent"
}
</script>

<style scoped>
.detail-box {
  background-color: #ececec;
  padding: 20px;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

.order-detail-image-item {
  display: flex;
  justify-content: flex-start;
  align-items: flex-start;
}

.order-detail-image-item img {
  border-radius: 8px;
  margin-right: 10px;
}

.order-sub-box {
  display: flex;
  flex-direction: row;
  justify-content: space-between;
  flex: 1;
  width: calc(100% - 90px); /* Adjust width to fit within the parent container, accounting for image width and margin */
}

.order-name {
  font-size: 18px;
  text-overflow: ellipsis;
  overflow: hidden;
  max-width: 100%; /* Adjust to fit within the parent container */
  white-space: nowrap;
  padding-right: 10px;
}

.order-price {
  flex-shrink: 0; /* Prevent the price from shrinking */
}

.price {
  font-size: 18px;
}

.list-box {
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  padding: 20px 0;
  font-size: 20px;
}

.price-calc {
  display: flex;
  justify-content: space-between;
}

.order-other{
  display: flex;
  justify-content: space-between;
}
</style>
