
/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: 89617663@qq.com
 */


#ifndef _BASE_SMART_PTR_T_H
#define _BASE_SMART_PTR_T_H

#include "base_common.h"
#include "base_thread_mutex.h"

NS_BASE_BEGIN


/**���ü�����*/
class RefCounter
{
public:
	RefCounter(void) : count_(0)
	{
	};
	
	virtual ~RefCounter(void)
	{
	};

	/**���Ӽ���*/
	int inc_ref(void)
	{
		Thread_Mutex_Guard guard(lock_);
		return ++count_;
	}
	
	/**���ټ���*/
	int dec_ref(void)
	{
		Thread_Mutex_Guard guard(lock_);
		if(count_>0)
		{
			--count_;
		}
		return count_;
	}

	//���ص�ǰ���ü���������
	int ref_count(void)
	{
		Thread_Mutex_Guard guard(lock_);
		return count_;
	};

private:
	int	count_;
	Thread_Mutex lock_;
};



/*
	Smart_Ptr_T ���ڶ��̳߳����¶���ĵĴ���
	T �����Ǽ̳���RefCounter
*/
template<typename T>
class Smart_Ptr_T
{
public:
	Smart_Ptr_T(T *ptr = NULL) : ptr_(ptr)
	{
		if(NULL == ptr_)
		{
			return;
		}
		ptr_->inc_ref();
	};

	Smart_Ptr_T(const Smart_Ptr_T<T> &obj) : ptr_(obj.ptr_)
	{
		//����������NULL��ֱ�ӷ���
		if(NULL == ptr_) 
		{
			return;
		}
		
		//ʹ��ͬһ��ָ�룬���ҽ����ü�����1
		ptr_->inc_ref();
	};


	template<typename R>
	Smart_Ptr_T(const Smart_Ptr_T<R> &obj) : ptr_((T*)obj.ptr_)
	{
		if(NULL == ptr_) 
		{
		  return;
		}
		ptr_->inc_ref();
	};



	//��������
	~Smart_Ptr_T(void)
	{
		if(NULL == ptr_) 
		{
		  return;
		}
		if(ptr_->dec_ref() == 0)
		{
			DELETE_POINTER(ptr_);
		}
	};


	//���ظ�ֵ����������ָ�븳ֵ
	Smart_Ptr_T<T>& operator=(T *ptr)
	{
	
		//�������ͬptr_��Smart_Ptr_T����ֵ�������󣬲���Ҫ�����ü�������
		if(ptr_ == ptr) 
		{
		  return *this;
		}

		if(NULL != ptr_)
		{
			if(ptr_->dec_ref() == 0)
			{
				DELETE_POINTER(ptr_);
			}
		}
		
		ptr_ = ptr;
		if(ptr_ != NULL) 
		{
			ptr_->inc_ref();
		}
		
		return *this;
	};




	//���ظ�ֵ����������Smart_Ptr_T<T>��ֵ
	Smart_Ptr_T<T>& operator=(const Smart_Ptr_T<T> &obj)
	{
		if(ptr_ == obj.ptr_)
		{
			return *this;
		}

		if(NULL != ptr_)
		{
			if(ptr_->dec_ref() == 0)
			{
				DELETE_POINTER(ptr_);	
			}
		}
		
		ptr_	= obj.ptr_;
		if(NULL != ptr_) 
		{
			ptr_->inc_ref();
		}
		
		return *this;
	};
	

	
	//���ظ�ֵ����������Smart_Ptr_T<R>��ֵ
	template<typename R>
	Smart_Ptr_T<T>& operator=(const Smart_Ptr_T<R> &obj)
	{
		if(ptr_ == obj.ptr_) 
		{
		  return *this;
		}

		if(NULL != ptr_)
		{
			if(ptr_->dec_ref() == 0) 
			{
				DELETE_POINTER(ptr_);
			}  
		}
		
		ptr_ = (T*)(obj.ptr_);
		
		if(NULL != ptr_)
		{
			ptr_->inc_ref();
		}

		return *this;
		
	}
	

	T* operator->(void)
	{
		return ptr_;
	};


	//const Smart_Ptr_T ��������ʹ�ã�ͬ��
	const T* operator->(void) const 
	{
		return ptr_;
	};



	//�þ�̬�������ظ�smart_ptr�ڲ�ָ��
	//���磺 if(Smart_Ptr_T::get_ptr(tcpss) == NULL) return -1;
	static T* get_ptr(const Smart_Ptr_T<T> &obj)
	{
		return obj.ptr_;
	};


	T* get_ptr(void)
	{
		return ptr_;
	};

	int get_ref_count(void)
	{
		if (ptr_ != NULL)
		{
			return ptr_->ref_count();
		}
	};

private:
	T	*ptr_;
	
};
	

NS_BASE_END


#endif

