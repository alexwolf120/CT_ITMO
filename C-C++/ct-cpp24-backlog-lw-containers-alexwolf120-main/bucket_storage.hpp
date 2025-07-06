#ifndef BUCKET_STORAGE_HPP
#define BUCKET_STORAGE_HPP

#include <assert.h>
#include <cmath>
#include <cstdint>

namespace bucket_storage
{

	template< typename T1, typename T2 >
	struct pair
	{
		T1 first_;
		T2 second_;

		pair(const T1& t1, const T2& t2) : first_(t1), second_(t2){};
		pair(T1&& t1, T2&& t2) : first_(std::move(t1)), second_(std::move(t2)){};
		~pair() = default;
	};

}	 // namespace bucket_storage

template< typename T >
class BucketStorage final
{
  public:
	template< bool is_const >
	class BucketStorageIterator;

	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using iterator = BucketStorageIterator< false >;
	using const_iterator = BucketStorageIterator< true >;
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;

  public:
	struct Block
	{
	  public:
		static const size_type all_later_free = SIZE_MAX;

		struct BlockElement final
		{
			value_type data_;
			size_type free_ = 0;

			BlockElement() = default;
			~BlockElement() = default;
			BlockElement(const BlockElement& other) : data_(other.data_), free_(other.free_) {}
			BlockElement(BlockElement&& other) noexcept : data_(std::move(other.data_)), free_(other.free_) {}
			BlockElement& operator=(BlockElement&& other)
			{
				if (this != &other)
				{
					new (&data_) value_type(std::move(other.data_));
					free_ = other.free_;
				}
				return *this;
			}
			BlockElement& operator=(const BlockElement& other)
			{
				if (this != &other)
				{
					new (&data_) value_type(other.data_);
					free_ = other.free_;
				}
				return *this;
			}
		};

		Block(size_type capacity) noexcept : capacity_(capacity)
		{
			elements_ = static_cast< BlockElement* >(malloc(capacity_ * sizeof(BlockElement)));
			for (size_type i = 0; i < capacity; ++i)
			{
				elements_[i].free_ = all_later_free;
			}
		}

		template< typename F >
		Block(size_type capacity, F&& data) : capacity_(capacity), size_(1), next_free_idx_(1)
		{
			elements_ = static_cast< BlockElement* >(malloc(capacity_ * sizeof(BlockElement)));
			try
			{
				new (&elements_[0].data_) value_type(std::forward< F >(data));
			} catch (...)
			{
				free(elements_);
				throw;
			}
			elements_[0].free_ = 0;
			for (size_type i = 1; i < capacity; ++i)
			{
				elements_[i].free_ = all_later_free;
			}
		}

		Block(const Block& other) { copy_block(other); }

		Block& operator=(const Block& other)
		{
			if (&other != this)
			{
				clear_elements();
				copy_block(other);
			}
			return *this;
		}

		Block& operator=(Block&& other)
		{
			if (&other != this)
			{
				clear_elements();
				capacity_ = other.capacity_;
				size_ = other.size_;
				next_free_idx_ = other.next_free_idx_;
				elements_ = other.elements_;
				other.elements_ = nullptr;
			}
		}

		Block(Block&& other)
		{
			capacity_ = other.capacity_;
			size_ = other.size_;
			next_free_idx_ = other.next_free_idx_;
			elements_ = other.elements_;
			other.elements_ = nullptr;
		}

		void clear_elements() noexcept
		{
			if (elements_)
			{
				for (size_type i = 0; i < capacity_; ++i)
				{
					if (elements_[i].free_ == 0)
					{
						elements_[i].data_.~value_type();
					}
				}
				free(elements_);
				elements_ = nullptr;
			}
		}

		~Block() { clear_elements(); }
		BlockElement& operator[](size_type index) noexcept { return elements_[index]; }

		bool have_space() const noexcept { return size_ != capacity_; }
		void copy_block(const Block& other)
		{
			capacity_ = other.capacity_;
			size_ = other.size_;
			next_free_idx_ = other.next_free_idx_;
			elements_ = static_cast< BlockElement* >(malloc(capacity_ * sizeof(BlockElement)));
			for (auto i = 0; i < capacity_; ++i)
			{
				elements_[i].free_ = other.elements_[i].free_;
				if (!other.elements_[i].free_)
				{
					new (&elements_[i].data_) value_type(other.elements_[i].data_);
				}
			}
		}
		template< class F >
		size_type insert(F&& value)
		{
			assert(elements_[next_free_idx_].free_ != 0);
			auto current_index = next_free_idx_;
			new (&elements_[current_index].data_) value_type(std::forward< F >(value));
			elements_[current_index].free_ = 0;
			++size_;
			if (elements_[current_index].free_ == 1)
			{
				if (have_space())
				{
					for (auto i = next_free_idx_ + 2; i < capacity_; ++i)
					{
						if (elements_[i].free_ != 0)
						{
							next_free_idx_ = i;
							break;
						}
					}
				}
				else
				{
					next_free_idx_ = capacity_;
				}
			}
			else
			{
				++next_free_idx_;
			}
			return current_index;
		};
		size_type erase(size_type index) noexcept
		{
			--size_;
			elements_[index].data_.~value_type();
			if (size_ == 0)
			{
				elements_[index].free_ = 1;
				return Block::all_later_free;
			}
			else
			{
				if (index == capacity_ - 1)
				{
					elements_[index].free_ = Block::all_later_free;
				}
				else
				{
					if (elements_[index + 1].free_ == Block::all_later_free)
					{
						elements_[index].free_ = Block::all_later_free;
					}
					else
					{
						elements_[index].free_ = elements_[index + 1].free_ + 1;
					}
				}
				if (next_free_idx_ > index)
				{
					next_free_idx_ = index;
				}
				else
				{
					for (size_type prev_idx = index - 1; prev_idx != Block::all_later_free; --prev_idx)
					{
						if (elements_[prev_idx].free_ != 0)
						{
							elements_[prev_idx].free_ += 1;
						}
						else
						{
							break;
						}
					}
				}
			}
			return index + elements_[index].free_;
		}

	  public:
		BlockElement* elements_ = nullptr;
		size_type capacity_ = 0;
		size_type size_ = 0;
		size_type next_free_idx_ = 0;
	};

	class BucketStorageElement
	{
		friend BucketStorage;

		BucketStorageElement(size_type capacity, size_type id) : block_(capacity), id_(id) {}
		template< typename F >
		BucketStorageElement(size_type capacity, F&& data, size_type id) :
			block_(capacity, std::forward< F >(data)), id_(id)
		{
		}
		BucketStorageElement(const BucketStorageElement& other) : block_(other.block_), id_(other.id_) {}
		BucketStorageElement(BucketStorageElement&& other) : block_(std::move(other.block_)), id_(other.id_) {}

		bool have_space() const noexcept { return block_.have_space(); }

	  private:
		BucketStorageElement* next_ = nullptr;
		BucketStorageElement* prev_ = nullptr;
		Block block_;
		size_type id_ = 0;
	};
	struct BlockWithSpaceElement
	{
		BucketStorageElement* element_ = nullptr;
		BlockWithSpaceElement* next_ = nullptr;

		BlockWithSpaceElement(BucketStorageElement* element, BlockWithSpaceElement* next = nullptr) noexcept :
			element_(element), next_(next)
		{
		}
		~BlockWithSpaceElement()
		{
			delete next_;
			next_ = nullptr;
		}
	};
	struct BucketStorageList
	{
		BucketStorageList(size_type block_capacity) : block_capacity_(block_capacity){};

		BucketStorageList(BucketStorageList&& other) noexcept { swap(std::move(other)); }

		BucketStorageList& operator=(BucketStorageList&& other) noexcept
		{
			if (&other != this)
			{
				swap(std::move(other));
			}
			return *this;
		}
		void swap(BucketStorageList&& other) noexcept
		{
			std::swap(head_, other.head_);
			std::swap(tail_, other.tail_);
			std::swap(blocks_with_space_, other.blocks_with_space_);
			std::swap(block_capacity_, other.block_capacity_);
		}

		BucketStorageList& operator=(const BucketStorageList& other)
		{
			if (&other != this)
			{
				clear();
				copyFrom(other);
			}
			return *this;
		}

		BucketStorageList(const BucketStorageList& other) { copyFrom(other); }
		auto block_count() const noexcept
		{
			auto count = 0;
			auto element = head_;
			while (element != nullptr)
			{
				++count;
				element = element->next_;
			}
			return count;
		}
		bool empty() const noexcept { return head_ == nullptr; };
		auto size() const noexcept
		{
			size_type size = 0;
			auto element = head_;
			while (element != nullptr)
			{
				size += element->block_.size_;
				element = element->next_;
			}
			return size;
		}

		template< class F >
		auto insert_into_list(F&& value)
		{
			BucketStorageElement* iter_element = nullptr;
			size_type iter_index = 0;
			if (blocks_with_space_ == nullptr)
			{
				if (empty())
				{
					head_ = new BucketStorageElement(block_capacity_, std::forward< F >(value), ++last_id_);
					tail_ = head_;
				}
				else
				{
					tail_->next_ = new BucketStorageElement(block_capacity_, std::forward< F >(value), ++last_id_);
					tail_->next_->prev_ = tail_;
					tail_ = tail_->next_;
				}
				if (tail_->have_space())
				{
					blocks_with_space_ = new BlockWithSpaceElement(tail_, blocks_with_space_);
				}
				iter_index = 0;
				iter_element = tail_;
			}
			else
			{
				auto current_block_with_space = blocks_with_space_;
				iter_element = current_block_with_space->element_;
				iter_index = iter_element->block_.insert(std::forward< F >(value));
				if (!current_block_with_space->element_->have_space())
				{
					blocks_with_space_ = current_block_with_space->next_;
					current_block_with_space->next_ = nullptr;
					delete current_block_with_space;
				}
			}
			return bucket_storage::pair< BucketStorageElement*, size_type >(iter_element, iter_index);
		}
		void clear() noexcept
		{
			delete blocks_with_space_;
			blocks_with_space_ = nullptr;
			auto next = head_;
			while (head_ != nullptr)
			{
				next = head_->next_;
				delete head_;
				head_ = next;
			}
			assert(head_ == nullptr);
		}

		~BucketStorageList() { clear(); }
		void reconstruct_blocks_with_space()
		{
			delete blocks_with_space_;
			blocks_with_space_ = nullptr;
			if (head_ == nullptr)
			{
				return;
			}
			for (auto list_element = head_; list_element != nullptr; list_element = list_element->next_)
			{
				if (list_element->block_.have_space())
				{
					blocks_with_space_ = new BlockWithSpaceElement(list_element, blocks_with_space_);
				}
			}
		}

	  private:
		void copyFrom(const BucketStorageList& other)
		{
			block_capacity_ = other.block_capacity_;
			if (other.head_ == nullptr)
			{
				return;
			}
			head_ = new BucketStorageElement(*other.head_);
			auto this_list_element = head_;
			for (auto other_list_element = other.head_->next_; other_list_element != nullptr;
				 other_list_element = other_list_element->next_)
			{
				assert(this_list_element->next_ == nullptr && "Memory can not be non-free here!");
				this_list_element->next_ = new BucketStorageElement(*other_list_element);
				this_list_element->next_->prev_ = this_list_element;
				this_list_element = this_list_element->next_;
			}
			tail_ = this_list_element;
			reconstruct_blocks_with_space();
		}

	  public:
		BucketStorageElement* head_ = nullptr;
		BucketStorageElement* tail_ = nullptr;
		BlockWithSpaceElement* blocks_with_space_ = nullptr;

		size_type block_capacity_;
		size_type last_id_ = 0;
	};

	class BucketStorageIteratorBase
	{
	  public:
		using size_type = BucketStorage::size_type;
		using difference_type = BucketStorage::difference_type;
		using iterator_category = std::bidirectional_iterator_tag;
		using distance_type = size_type;

		friend BucketStorage;

	  public:
		BucketStorageIteratorBase(const BucketStorage* storage, BucketStorageElement* element, size_type index) :
			storage_(storage), element_(element), index_(index)
		{
			end_flag = is_end();
		}

		bool is_end() const
		{
			return end_flag || element_ == nullptr || (element_->next_ == nullptr && index_ == element_->block_.capacity_);
		}

		bool operator==(const BucketStorageIteratorBase& other) const
		{
			return ((is_end() && other.is_end()) && (storage_ == other.storage_)) ||
				   (element_ == other.element_ && index_ == other.index_);
		}
		bool operator!=(const BucketStorageIteratorBase& other) const { return !(operator==(other)); }
		bool operator>(const BucketStorageIteratorBase& other)
		{
			if (is_end())
			{
				return false;
			}
			else
			{
				if (other.is_end())
				{
					return true;
				}
			}
			if (other.element_->id_ > element_->id_)
			{
				return true;
			}
			else if (other.element_->id_ < element_->id_)
			{
				return false;
			}
			else
			{
				return index_ < other.index_;
			}
		}
		bool operator<(const BucketStorageIteratorBase& other) { return !operator>=(other); }
		bool operator>=(const BucketStorageIteratorBase& other) { return operator>(other) || operator==(other); }
		bool operator<=(const BucketStorageIteratorBase& other) { return !operator>(other); }

	  protected:
		const BucketStorage* storage_ = nullptr;
		BucketStorageElement* element_ = nullptr;
		size_type index_ = 0;
		bool end_flag = false;
	};

	template< bool is_const >
	class BucketStorageIterator : public BucketStorageIteratorBase
	{
	  public:
		using value_type = std::conditional_t< is_const, const BucketStorage::value_type, BucketStorage::value_type >;
		using pointer = std::conditional_t< is_const, const value_type*, value_type* >;
		using reference = std::conditional_t< is_const, const value_type&, value_type& >;

		BucketStorageIterator(const BucketStorage* storage, BucketStorageElement* element, size_type index) :
			BucketStorageIteratorBase(storage, element, index)
		{
		}

		operator BucketStorageIterator< true >()
		{
			return BucketStorageIterator< true >(this->storage_, this->element_, this->index_);
		}

		explicit operator BucketStorageIterator< false >()
		{
			return BucketStorageIterator< false >(this->storage_, this->element_, this->index_);
		}

		operator BucketStorageIterator< true >() const
		{
			return BucketStorageIterator< true >(this->storage_, this->element_, this->index_);
		}

		explicit operator BucketStorageIterator< false >() const
		{
			return BucketStorageIterator< false >(this->storage_, this->element_, this->index_);
		}

		BucketStorageIterator& operator++()
		{
			auto& index_ = this->index_;
			auto& element_ = this->element_;
			auto& end_flag = this->end_flag;
			if (this->is_end())
			{
				return *this;
			}
			++index_;
			if ((index_ != element_->block_.capacity_) && (element_->block_[index_].free_ == Block::all_later_free))
			{
				index_ = element_->block_.capacity_;
			}
			if (this->is_end())
			{
				end_flag = true;
				return *this;
			}
			if (index_ == element_->block_.capacity_)
			{
				element_ = element_->next_;
				index_ = 0;
			}
			index_ += element_->block_[index_].free_;
			assert(index_ != Block::all_later_free);
			return *this;
		}

		BucketStorageIterator operator++(int) noexcept
		{
			BucketStorageIterator iter(*this);
			operator++();
			return iter;
		}

		BucketStorageIterator& operator--()
		{
			auto& index_ = this->index_;
			auto& element_ = this->element_;
			auto& end_flag = this->end_flag;
			auto& storage_ = this->storage_;
			if (this->is_end())
			{
				if (storage_->empty())
				{
					return *this;
				}
				element_ = storage_->block_list_.tail_;
				index_ = storage_->block_capacity_;
			}
			if (index_ == 0)
			{
				if (element_->prev_ == nullptr)
				{
					return *this;
				}
				else
				{
					element_ = element_->prev_;
					index_ = element_->block_.capacity_;
				}
			}
			do
			{
				--index_;
			} while (element_->block_[index_].free_);
			end_flag = false;
			return *this;
		}

		BucketStorageIterator operator--(int) noexcept
		{
			BucketStorageIterator iter(*this);
			operator--();
			return iter;
		}

		BucketStorageIterator& operator=(const BucketStorageIterator& other) = default;

		reference operator*() { return this->element_->block_[this->index_].data_; }
		auto operator->() { return &(operator*()); }
	};

  public:
	BucketStorage() : block_list_(block_capacity_) {}
	BucketStorage(size_type block_capacity) : block_capacity_(block_capacity), block_list_(block_capacity)
	{
		assert(block_capacity < Block::all_later_free);
	}
	~BucketStorage() {}
	BucketStorage(BucketStorage&& other) noexcept :
		block_list_(std::move(other.block_list_)), block_capacity_(std::move(other.block_capacity_))
	{
		other.block_list_.clear();
	}
	BucketStorage(const BucketStorage& other) : block_list_(other.block_list_), block_capacity_(other.block_capacity_)
	{
	}
	BucketStorage< T >& operator=(BucketStorage&& other) noexcept
	{
		if (&other != this)
		{
			block_capacity_ = other.block_capacity_;
			block_list_ = std::move(other.block_list_);
			other.block_list_.clear();
		}
		return *this;
	}
	BucketStorage< T >& operator=(const BucketStorage& other)
	{
		if (&other != this)
		{
			block_capacity_ = other.block_capacity_;
			block_list_ = other.block_list_;
		}
		return *this;
	}
	iterator insert(const value_type& value)
	{
		auto data = block_list_.insert_into_list(value);
		return iterator(this, data.first_, data.second_);
	}
	iterator insert(value_type&& value)
	{
		auto data = block_list_.insert_into_list(std::move(value));
		return iterator(this, data.first_, data.second_);
	}
	iterator erase(const_iterator it)
	{
		iterator new_it = static_cast< iterator >(it);
		if (it.is_end())
		{
			return new_it;
		}
		++new_it;
		it.element_->block_.erase(it.index_);
		if (it.element_->block_.size_ == 0)
		{
			if (it.element_->next_)
			{
				it.element_->next_->prev_ = it.element_->prev_;
			}
			else
			{
				block_list_.tail_ = block_list_.tail_->prev_;
			}
			if (it.element_->prev_)
			{
				it.element_->prev_->next_ = it.element_->next_;
			}
			else
			{
				block_list_.head_ = block_list_.head_->next_;
			}
			auto find_block = block_list_.blocks_with_space_;
			if (find_block->element_ == it.element_)
			{
				block_list_.blocks_with_space_ = find_block->next_;
			}
			else
			{
				while (find_block->next_->element_ != it.element_)
				{
					find_block = find_block->next_;
				}
				auto tmp = find_block->next_;
				find_block->next_ = find_block->next_->next_;
				find_block = tmp;
			}
			assert(find_block != nullptr);
			find_block->next_ = nullptr;
			delete find_block;
			delete it.element_;
		}
		else
		{
			if (it.element_->block_.size_ == it.element_->block_.capacity_ - 1)
			{
				block_list_.blocks_with_space_ = new BlockWithSpaceElement(it.element_, block_list_.blocks_with_space_);
			}
		}
		return new_it;
	}
	void shrink_to_fit()
	{
		auto blocks_needed = block_list_.size() / block_capacity_;
		if (block_list_.size() % block_capacity_)
		{
			++blocks_needed;
		}
		if (block_list_.block_count() > blocks_needed)
		{
			BucketStorageList new_block_list_(block_capacity_);
			while (!empty())
			{
				auto it = begin();
				new_block_list_.insert_into_list(std::move(*it));
				erase(it);
			}
			block_list_ = std::move(new_block_list_);
		}
	}

	void clear() noexcept { block_list_.clear(); }

	bool empty() const noexcept { return block_list_.empty(); };

	size_type size() const noexcept { return block_list_.size(); };

	size_type capacity() const noexcept { return block_list_.block_count() * block_capacity_; };

	void swap(BucketStorage& other) noexcept
	{
		if (this != &other)
		{
			std::swap(block_list_, other.block_list_);
			std::swap(block_capacity_, other.block_capacity_);
			block_list_.reconstruct_blocks_with_space();
			other.block_list_.reconstruct_blocks_with_space();
		}
	}

	iterator begin() noexcept
	{
		if (block_list_.head_ == nullptr)
		{
			return iterator(this, nullptr, 0);
		}
		else
		{
			size_type index = block_list_.head_->block_[0].free_;
			return iterator(this, block_list_.head_, index);
		}
	}

	const_iterator begin() const noexcept
	{
		if (block_list_.head_ == nullptr)
		{
			return iterator(this, nullptr, 0);
		}
		else
		{
			size_type index = block_list_.head_->block_[0].free_;
			return iterator(this, block_list_.head_, index);
		}
	}

	const_iterator cbegin() const noexcept { return begin(); }

	iterator end() noexcept { return iterator(this, nullptr, 0); }

	const_iterator end() const noexcept { return const_iterator(this, nullptr, 0); }

	const_iterator cend() const noexcept { return end(); }

	iterator get_to_distance(iterator it, const difference_type distance) noexcept
	{
		for (auto i = 0; i < std::abs(distance); ++i)
		{
			if (distance > 0)
			{
				++it;
			}
			else
			{
				--it;
			}
		}
		return it;
	}

  private:
	size_type block_capacity_ = 64;
	BucketStorageList block_list_;
};

#endif	  // BUCKET_STORAGE_HPP
