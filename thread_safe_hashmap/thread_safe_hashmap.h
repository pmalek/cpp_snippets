#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <shared_mutex>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class thread_safe_hashmap
{
  class bucket_type
  {
    template <typename Key2, typename Value2>
    friend std::ostream& operator<<(std::ostream& os, const thread_safe_hashmap<Key2, Value2>& map);

    typedef std::pair<Key, Value> bucket_value;
    typedef std::list<bucket_value> bucket_data;
    typedef typename bucket_data::iterator bucket_iterator;
    typedef typename bucket_data::const_iterator bucket_const_iterator;

    bucket_data data;
    mutable std::shared_timed_mutex mutex;

    bucket_iterator find_entry_for(const Key& key)
    {
      return std::find_if(data.begin(), data.end(),
                          [&](const bucket_value& item) { return item.first == key; });
    }

    bucket_const_iterator find_entry_for(const Key& key) const
    {
      return std::find_if(data.begin(), data.end(),
                          [&](const bucket_value& item) { return item.first == key; });
    }

  public:
    Value value_for(Key const& key, Value const& default_value) const
    {
      std::shared_lock<std::shared_timed_mutex> lock(mutex);
      bucket_const_iterator found_entry = find_entry_for(key);
      return (found_entry == data.end()) ? default_value : found_entry->second;
    }

    void add_or_update_mapping(Key const& key, Value const& value)
    {
      std::unique_lock<std::shared_timed_mutex> lock(mutex);
      bucket_iterator found_entry = find_entry_for(key);
      if (found_entry == data.end())
        data.push_back(bucket_value(key, value));
      else
        found_entry->second = value;
    }

    void remove_mapping(Key const& key)
    {
      std::unique_lock<std::shared_timed_mutex> lock(mutex);
      bucket_iterator found_entry = find_entry_for(key);
      if (found_entry != data.end())
        data.erase(found_entry);
    }
  };

  std::vector<std::unique_ptr<bucket_type>> buckets;
  Hash hasher;

  bucket_type& get_bucket(Key const& key) const
  {
    std::size_t const bucket_index = hasher(key) % buckets.size();
    return *buckets[bucket_index];
  }

  template <typename Key2, typename Value2>
  friend std::ostream& operator<<(std::ostream& os, const thread_safe_hashmap<Key2, Value2>& map);

public:
  typedef Key key_type;
  typedef Value mapped_type;
  typedef Hash hash_type;

  thread_safe_hashmap(unsigned num_buckets = 19, Hash const& hasher_ = Hash())
      : buckets(num_buckets), hasher(hasher_)
  {
    for (unsigned i = 0; i < num_buckets; ++i)
      buckets[i].reset(new bucket_type);
  }

  thread_safe_hashmap(thread_safe_hashmap const& other) = delete;
  thread_safe_hashmap& operator=(thread_safe_hashmap const& other) = delete;

  Value value_for(Key const& key, Value const& default_value = Value()) const
  {
    return get_bucket(key).value_for(key, default_value);
  }

  void add_or_update_mapping(Key const& key, Value const& value)
  {
    get_bucket(key).add_or_update_mapping(key, value);
  }

  void remove_mapping(Key const& key)
  {
    get_bucket(key).remove_mapping(key);
  }
};

template <typename First, typename Second>
std::ostream& operator<<(std::ostream& os, const std::pair<First, Second>& p)
{
  os << p.first << ' ' << p.second;
  return os;
}

template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const thread_safe_hashmap<Key, Value>& map)
{
  for (const auto& bucket : map.buckets)
    for (const auto el : bucket->data) os << el << '\n';

  return os;
}
