class ECommercePlatform:
    """
    电子商务平台类，用于管理产品、订单、支付和用户服务
    """
    
    def __init__(self):
        self.users = {}  # 用户ID到用户信息的映射
        self.products = {}  # 产品ID到产品信息的映射
        self.categories = {}  # 类别ID到类别信息的映射
        self.orders = {}  # 订单ID到订单信息的映射
        self.payments = {}  # 支付ID到支付信息的映射
        self.shipments = {}  # 运输ID到运输信息的映射
        self.reviews = {}  # 评论ID到评论信息的映射
        self.shopping_carts = {}  # 购物车ID到购物车信息的映射
        self.wishlists = {}  # 愿望清单ID到愿望清单信息的映射
    
    def create_user(self, user_id, username, email, password, shipping_address=None, billing_address=None):
        """
        创建新用户
        @param user_id: 用户唯一标识符
        @param username: 用户名
        @param email: 电子邮件地址
        @param password: 密码
        @param shipping_address: 配送地址（可选）
        @param billing_address: 账单地址（可选）
        @return: 创建成功返回True，否则返回False
        """
        if user_id in self.users:
            return False
        
        if any(user['email'] == email for user in self.users.values()):
            return False
        
        if any(user['username'] == username for user in self.users.values()):
            return False
        
        self.users[user_id] = {
            'username': username,
            'email': email,
            'password': password,  # 实际应用中应该使用哈希存储密码
            'shipping_address': shipping_address,
            'billing_address': billing_address,
            'orders': [],
            'shopping_cart_id': None,
            'wishlist_id': None,
            'created_at': self.get_current_timestamp()
        }
        
        # 为用户创建购物车和愿望清单
        shopping_cart_id = self.generate_id('cart')
        self.shopping_carts[shopping_cart_id] = {
            'user_id': user_id,
            'items': [],
            'created_at': self.get_current_timestamp(),
            'updated_at': self.get_current_timestamp()
        }
        self.users[user_id]['shopping_cart_id'] = shopping_cart_id
        
        wishlist_id = self.generate_id('wishlist')
        self.wishlists[wishlist_id] = {
            'user_id': user_id,
            'items': [],
            'created_at': self.get_current_timestamp(),
            'updated_at': self.get_current_timestamp()
        }
        self.users[user_id]['wishlist_id'] = wishlist_id
        
        return True
    
    def create_category(self, category_id, name, description=None, parent_id=None):
        """
        创建产品类别
        @param category_id: 类别唯一标识符
        @param name: 类别名称
        @param description: 类别描述（可选）
        @param parent_id: 父类别ID（可选）
        @return: 创建成功返回True，否则返回False
        """
        if category_id in self.categories:
            return False
        
        # 检查父类别是否存在
        if parent_id is not None and parent_id not in self.categories:
            return False
        
        self.categories[category_id] = {
            'name': name,
            'description': description,
            'parent_id': parent_id,
            'created_at': self.get_current_timestamp()
        }
        
        return True
    
    def create_product(self, product_id, name, description, price, category_id, stock_quantity, 
                       sku=None, weight=None, dimensions=None, images=None):
        """
        创建新产品
        @param product_id: 产品唯一标识符
        @param name: 产品名称
        @param description: 产品描述
        @param price: 产品价格
        @param category_id: 产品类别ID
        @param stock_quantity: 库存数量
        @param sku: SKU（可选）
        @param weight: 产品重量（可选）
        @param dimensions: 产品尺寸（可选）
        @param images: 图片URL列表（可选）
        @return: 创建成功返回True，否则返回False
        """
        if product_id in self.products:
            return False
        
        if category_id not in self.categories:
            return False
        
        self.products[product_id] = {
            'name': name,
            'description': description,
            'price': price,
            'category_id': category_id,
            'stock_quantity': stock_quantity,
            'sku': sku or f"SKU-{product_id}",
            'weight': weight,
            'dimensions': dimensions,
            'images': images or [],
            'created_at': self.get_current_timestamp(),
            'updated_at': self.get_current_timestamp()
        }
        
        return True
    
    def add_to_cart(self, user_id, product_id, quantity):
        """
        添加产品到购物车
        @param user_id: 用户ID
        @param product_id: 产品ID
        @param quantity: 数量
        @return: 添加成功返回True，否则返回False
        """
        if user_id not in self.users or product_id not in self.products:
            return False
        
        if quantity <= 0:
            return False
        
        if self.products[product_id]['stock_quantity'] < quantity:
            return False  # 库存不足
        
        shopping_cart_id = self.users[user_id]['shopping_cart_id']
        if shopping_cart_id not in self.shopping_carts:
            return False
        
        shopping_cart = self.shopping_carts[shopping_cart_id]
        
        # 检查产品是否已在购物车中
        for item in shopping_cart['items']:
            if item['product_id'] == product_id:
                item['quantity'] += quantity
                shopping_cart['updated_at'] = self.get_current_timestamp()
                return True
        
        # 添加新产品到购物车
        shopping_cart['items'].append({
            'product_id': product_id,
            'quantity': quantity,
            'added_at': self.get_current_timestamp()
        })
        shopping_cart['updated_at'] = self.get_current_timestamp()
        
        return True
    
    def remove_from_cart(self, user_id, product_id):
        """
        从购物车中移除产品
        @param user_id: 用户ID
        @param product_id: 产品ID
        @return: 移除成功返回True，否则返回False
        """
        if user_id not in self.users or product_id not in self.products:
            return False
        
        shopping_cart_id = self.users[user_id]['shopping_cart_id']
        if shopping_cart_id not in self.shopping_carts:
            return False
        
        shopping_cart = self.shopping_carts[shopping_cart_id]
        
        # 查找并移除产品
        for i, item in enumerate(shopping_cart['items']):
            if item['product_id'] == product_id:
                shopping_cart['items'].pop(i)
                shopping_cart['updated_at'] = self.get_current_timestamp()
                return True
        
        return False  # 产品不在购物车中
    
    def update_cart_quantity(self, user_id, product_id, new_quantity):
        """
        更新购物车中产品的数量
        @param user_id: 用户ID
        @param product_id: 产品ID
        @param new_quantity: 新数量
        @return: 更新成功返回True，否则返回False
        """
        if user_id not in self.users or product_id not in self.products:
            return False
        
        if new_quantity <= 0:
            return False
        
        if self.products[product_id]['stock_quantity'] < new_quantity:
            return False  # 库存不足
        
        shopping_cart_id = self.users[user_id]['shopping_cart_id']
        if shopping_cart_id not in self.shopping_carts:
            return False
        
        shopping_cart = self.shopping_carts[shopping_cart_id]
        
        # 查找并更新产品数量
        for item in shopping_cart['items']:
            if item['product_id'] == product_id:
                item['quantity'] = new_quantity
                shopping_cart['updated_at'] = self.get_current_timestamp()
                return True
        
        return False  # 产品不在购物车中
    
    def get_shopping_cart(self, user_id):
        """
        获取用户购物车
        @param user_id: 用户ID
        @return: 购物车信息，用户不存在返回None
        """
        if user_id not in self.users:
            return None
        
        shopping_cart_id = self.users[user_id]['shopping_cart_id']
        if shopping_cart_id not in self.shopping_carts:
            return None
        
        shopping_cart = self.shopping_carts[shopping_cart_id].copy()
        
        # 添加产品详细信息
        items_with_details = []
        for item in shopping_cart['items']:
            product_id = item['product_id']
            if product_id in self.products:
                product_info = self.products[product_id].copy()
                product_info['quantity'] = item['quantity']
                product_info['subtotal'] = product_info['price'] * item['quantity']
                items_with_details.append(product_info)
        
        shopping_cart['items'] = items_with_details
        
        # 计算总价
        shopping_cart['total_price'] = sum(item['subtotal'] for item in items_with_details)
        
        return shopping_cart
    
    def add_to_wishlist(self, user_id, product_id):
        """
        添加产品到愿望清单
        @param user_id: 用户ID
        @param product_id: 产品ID
        @return: 添加成功返回True，否则返回False
        """
        if user_id not in self.users or product_id not in self.products:
            return False
        
        wishlist_id = self.users[user_id]['wishlist_id']
        if wishlist_id not in self.wishlists:
            return False
        
        wishlist = self.wishlists[wishlist_id]
        
        # 检查产品是否已在愿望清单中
        if any(item['product_id'] == product_id for item in wishlist['items']):
            return False  # 产品已在愿望清单中
        
        # 添加新产品到愿望清单
        wishlist['items'].append({
            'product_id': product_id,
            'added_at': self.get_current_timestamp()
        })
        wishlist['updated_at'] = self.get_current_timestamp()
        
        return True
    
    def remove_from_wishlist(self, user_id, product_id):
        """
        从愿望清单中移除产品
        @param user_id: 用户ID
        @param product_id: 产品ID
        @return: 移除成功返回True，否则返回False
        """
        if user_id not in self.users or product_id not in self.products:
            return False
        
        wishlist_id = self.users[user_id]['wishlist_id']
        if wishlist_id not in self.wishlists:
            return False
        
        wishlist = self.wishlists[wishlist_id]
        
        # 查找并移除产品
        for i, item in enumerate(wishlist['items']):
            if item['product_id'] == product_id:
                wishlist['items'].pop(i)
                wishlist['updated_at'] = self.get_current_timestamp()
                return True
        
        return False  # 产品不在愿望清单中
    
    def get_wishlist(self, user_id):
        """
        获取用户愿望清单
        @param user_id: 用户ID
        @return: 愿望清单信息，用户不存在返回None
        """
        if user_id not in self.users:
            return None
        
        wishlist_id = self.users[user_id]['wishlist_id']
        if wishlist_id not in self.wishlists:
            return None
        
        wishlist = self.wishlists[wishlist_id].copy()
        
        # 添加产品详细信息
        items_with_details = []
        for item in wishlist['items']:
            product_id = item['product_id']
            if product_id in self.products:
                product_info = self.products[product_id].copy()
                product_info['added_at'] = item['added_at']
                items_with_details.append(product_info)
        
        wishlist['items'] = items_with_details
        
        return wishlist
    
    def create_order(self, user_id, shipping_address=None, billing_address=None):
        """
        创建订单
        @param user_id: 用户ID
        @param shipping_address: 配送地址（可选）
        @param billing_address: 账单地址（可选）
        @return: 创建成功返回订单ID，否则返回None
        """
        if user_id not in self.users:
            return None
        
        shopping_cart = self.get_shopping_cart(user_id)
        if not shopping_cart or not shopping_cart['items']:
            return None  # 购物车为空
        
        # 使用提供的地址或用户默认地址
        user = self.users[user_id]
        shipping_address = shipping_address or user.get('shipping_address')
        billing_address = billing_address or user.get('billing_address') or shipping_address
        
        if not shipping_address:
            return None  # 缺少配送地址
        
        # 创建订单
        order_id = self.generate_id('order')
        order_items = []
        
        for item in shopping_cart['items']:
            product_id = item['product_id']
            quantity = item['quantity']
            
            # 检查库存
            if self.products[product_id]['stock_quantity'] < quantity:
                return None  # 库存不足
            
            # 更新库存
            self.products[product_id]['stock_quantity'] -= quantity
            self.products[product_id]['updated_at'] = self.get_current_timestamp()
            
            # 添加订单项
            order_items.append({
                'product_id': product_id,
                'quantity': quantity,
                'price': item['price'],
                'subtotal': item['subtotal']
            })
        
        self.orders[order_id] = {
            'user_id': user_id,
            'items': order_items,
            'total_price': shopping_cart['total_price'],
            'status': 'pending',  # pending, paid, shipped, delivered, cancelled
            'shipping_address': shipping_address,
            'billing_address': billing_address,
            'created_at': self.get_current_timestamp(),
            'updated_at': self.get_current_timestamp()
        }
        
        # 更新用户的订单列表
        self.users[user_id]['orders'].append(order_id)
        
        # 清空购物车
        self.shopping_carts[self.users[user_id]['shopping_cart_id']]['items'] = []
        self.shopping_carts[self.users[user_id]['shopping_cart_id']]['updated_at'] = self.get_current_timestamp()
        
        return order_id
    
    def process_payment(self, order_id, payment_method, payment_details):
        """
        处理订单支付
        @param order_id: 订单ID
        @param payment_method: 支付方式
        @param payment_details: 支付详情
        @return: 支付成功返回支付ID，否则返回None
        """
        if order_id not in self.orders:
            return None
        
        order = self.orders[order_id]
        if order['status'] != 'pending':
            return None  # 订单状态不正确
        
        # 创建支付记录
        payment_id = self.generate_id('payment')
        self.payments[payment_id] = {
            'order_id': order_id,
            'amount': order['total_price'],
            'payment_method': payment_method,
            'payment_details': payment_details,
            'status': 'success',  # success, failed, pending
            'created_at': self.get_current_timestamp()
        }
        
        # 更新订单状态
        order['status'] = 'paid'
        order['updated_at'] = self.get_current_timestamp()
        
        return payment_id
    
    def ship_order(self, order_id, shipping_method, tracking_number=None):
        """
        配送订单
        @param order_id: 订单ID
        @param shipping_method: 配送方式
        @param tracking_number: 追踪号码（可选）
        @return: 配送成功返回运输ID，否则返回None
        """
        if order_id not in self.orders:
            return None
        
        order = self.orders[order_id]
        if order['status'] != 'paid':
            return None  # 订单状态不正确
        
        # 创建运输记录
        shipment_id = self.generate_id('shipment')
        self.shipments[shipment_id] = {
            'order_id': order_id,
            'shipping_method': shipping_method,
            'tracking_number': tracking_number,
            'status': 'shipped',  # shipped, in_transit, delivered, returned
            'shipped_at': self.get_current_timestamp(),
            'estimated_delivery': self.calculate_estimated_delivery(shipping_method)
        }
        
        # 更新订单状态
        order['status'] = 'shipped'
        order['updated_at'] = self.get_current_timestamp()
        
        return shipment_id
    
    def deliver_order(self, order_id):
        """
        标记订单为已送达
        @param order_id: 订单ID
        @return: 更新成功返回True，否则返回False
        """
        if order_id not in self.orders:
            return False
        
        order = self.orders[order_id]
        if order['status'] != 'shipped':
            return False  # 订单状态不正确
        
        # 更新订单状态
        order['status'] = 'delivered'
        order['updated_at'] = self.get_current_timestamp()
        
        # 更新运输记录状态
        for shipment_id, shipment in self.shipments.items():
            if shipment['order_id'] == order_id and shipment['status'] != 'delivered':
                shipment['status'] = 'delivered'
                shipment['delivered_at'] = self.get_current_timestamp()
                break
        
        return True
    
    def cancel_order(self, order_id, reason=""):
        """
        取消订单
        @param order_id: 订单ID
        @param reason: 取消原因
        @return: 取消成功返回True，否则返回False
        """
        if order_id not in self.orders:
            return False
        
        order = self.orders[order_id]
        if order['status'] not in ['pending', 'paid']:
            return False  # 订单状态不正确
        
        # 更新订单状态
        order['status'] = 'cancelled'
        order['updated_at'] = self.get_current_timestamp()
        order['cancellation_reason'] = reason
        
        # 如果订单已支付，处理退款（简化版）
        if order['status'] == 'paid':
            # 在实际应用中，这里会有更复杂的退款处理逻辑
            pass
        
        # 恢复库存
        for item in order['items']:
            product_id = item['product_id']
            if product_id in self.products:
                self.products[product_id]['stock_quantity'] += item['quantity']
                self.products[product_id]['updated_at'] = self.get_current_timestamp()
        
        return True
    
    def add_product_review(self, user_id, product_id, rating, comment=""):
        """
        添加产品评论
        @param user_id: 用户ID
        @param product_id: 产品ID
        @param rating: 评分（1-5）
        @param comment: 评论内容（可选）
        @return: 评论成功返回评论ID，否则返回None
        """
        if user_id not in self.users or product_id not in self.products:
            return None
        
        if rating < 1 or rating > 5:
            return None
        
        # 检查用户是否已购买该产品
        user_orders = [self.orders[order_id] for order_id in self.users[user_id]['orders'] if order_id in self.orders]
        has_purchased = False
        
        for order in user_orders:
            if order['status'] in ['paid', 'shipped', 'delivered']:
                for item in order['items']:
                    if item['product_id'] == product_id:
                        has_purchased = True
                        break
        
        if not has_purchased:
            return None  # 用户未购买该产品，不能评论
        
        # 检查用户是否已评论过该产品
        for review_id, review in self.reviews.items():
            if review['user_id'] == user_id and review['product_id'] == product_id:
                return None  # 用户已评论过该产品
        
        # 创建评论
        review_id = self.generate_id('review')
        self.reviews[review_id] = {
            'user_id': user_id,
            'product_id': product_id,
            'rating': rating,
            'comment': comment,
            'created_at': self.get_current_timestamp()
        }
        
        return review_id
    
    def get_product_reviews(self, product_id, limit=10):
        """
        获取产品评论
        @param product_id: 产品ID
        @param limit: 返回评论数量限制
        @return: 评论列表
        """
        if product_id not in self.products:
            return []
        
        result = []
        for review_id, review in self.reviews.items():
            if review['product_id'] == product_id:
                review_copy = review.copy()
                review_copy['review_id'] = review_id
                review_copy['username'] = self.users[review['user_id']]['username']
                result.append(review_copy)
        
        # 按创建时间降序排序
        result.sort(key=lambda x: x['created_at'], reverse=True)
        
        if len(result) > limit:
            result = result[:limit]
        
        return result
    
    def get_product_details(self, product_id):
        """
        获取产品详细信息
        @param product_id: 产品ID
        @return: 产品详细信息，产品不存在返回None
        """
        if product_id not in self.products:
            return None
        
        product = self.products[product_id].copy()
        
        # 添加类别信息
        category_id = product['category_id']
        if category_id in self.categories:
            product['category'] = self.categories[category_id]['name']
        
        # 计算平均评分
        reviews = self.get_product_reviews(product_id, limit=1000)
        if reviews:
            average_rating = sum(review['rating'] for review in reviews) / len(reviews)
            product['average_rating'] = round(average_rating, 2)
            product['review_count'] = len(reviews)
        else:
            product['average_rating'] = 0
            product['review_count'] = 0
        
        return product
    
    def search_products(self, keyword, category_id=None, min_price=None, max_price=None):
        """
        搜索产品
        @param keyword: 搜索关键词
        @param category_id: 类别筛选（可选）
        @param min_price: 最低价格（可选）
        @param max_price: 最高价格（可选）
        @return: 匹配的产品列表
        """
        keyword = keyword.lower()
        result = []
        
        for product_id, product in self.products.items():
            # 检查关键词匹配
            if (keyword in product['name'].lower() or 
                keyword in product['description'].lower() or
                keyword in product.get('sku', '').lower()):
                
                # 检查类别匹配
                if category_id is not None and product['category_id'] != category_id:
                    continue
                
                # 检查价格范围
                price = product['price']
                if min_price is not None and price < min_price:
                    continue
                
                if max_price is not None and price > max_price:
                    continue
                
                product_copy = self.get_product_details(product_id)
                if product_copy:
                    result.append(product_copy)
        
        return result
    
    def get_user_orders(self, user_id, status=None, limit=10):
        """
        获取用户订单
        @param user_id: 用户ID
        @param status: 订单状态筛选（可选）
        @param limit: 返回订单数量限制
        @return: 订单列表
        """
        if user_id not in self.users:
            return []
        
        result = []
        for order_id in self.users[user_id]['orders']:
            if order_id in self.orders:
                order = self.orders[order_id].copy()
                order['order_id'] = order_id
                
                if status is not None and order['status'] != status:
                    continue
                
                result.append(order)
        
        # 按创建时间降序排序
        result.sort(key=lambda x: x['created_at'], reverse=True)
        
        if len(result) > limit:
            result = result[:limit]
        
        return result
    
    def calculate_estimated_delivery(self, shipping_method):
        """
        计算预计送达时间
        @param shipping_method: 配送方式
        @return: 预计送达时间
        """
        import datetime
        
        current_time = datetime.datetime.now()
        
        if shipping_method == 'standard':
            estimated_delivery = current_time + datetime.timedelta(days=5)
        elif shipping_method == 'express':
            estimated_delivery = current_time + datetime.timedelta(days=2)
        elif shipping_method == 'overnight':
            estimated_delivery = current_time + datetime.timedelta(days=1)
        else:
            estimated_delivery = current_time + datetime.timedelta(days=7)
        
        return estimated_delivery.isoformat()
    
    def generate_id(self, prefix):
        """
        生成唯一ID
        @param prefix: ID前缀
        @return: 唯一ID字符串
        """
        import uuid
        return f"{prefix}_{uuid.uuid4().hex[:8]}"
    
    def get_current_timestamp(self):
        """
        获取当前时间戳
        @return: 当前时间戳字符串
        """
        import datetime
        return datetime.datetime.now().isoformat()