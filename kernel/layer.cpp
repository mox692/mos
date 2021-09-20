#include "layer.hpp"

#include <algorithm>

Layer::Layer(unsigned int id) : id_{id} {
}

unsigned int Layer::ID() const {
  return id_;
}

// Laylerに、引数で受け取ったwindowを登録する.
Layer& Layer::SetWindow(const std::shared_ptr<Window>& window) {
  // TODO: こうやってメンバ変数にアクセスできるの？
  window_ = window;
  // MEMO: thisはRustで言うself?(インスタンス変数自身)
  return *this;
}

std::shared_ptr<Window> Layer::GetWindow() const {
  return window_;
}

// MEMO: Layler.posを更新する.
Layer& Layer::Move(Vector2D<int> pos) {
  pos_ = pos;
  return *this;
}

Layer& Layer::MoveRelative(Vector2D<int> pos_diff) {
  pos_ += pos_diff;
  return *this;
}

// layerに紐づいたwindowがあるかを確認して、あればwindowクラスのDrawToに処理を以上する.
// MEMO: この実装から、layerに直接writeすることはできないことがわかる.
void Layer::DrawTo(FrameBuffer& screen) const {
  if (window_) {
    window_->DrawTo(screen, pos_);
  }
}


void LayerManager::SetWriter(FrameBuffer* screen) {
  screen_ = screen;
}

// 新しいLaylerを作成し、layersにappendする.
// この際、latest_id_をincrementしてあげる.
Layer& LayerManager::NewLayer() {
  ++latest_id_;
  return *layers_.emplace_back(new Layer{latest_id_});
}

// MEMO: layer_stack_に溜まっているlayerを全て描画する.
// 実際の処理はLayer classのDrawToに委譲する.
void LayerManager::Draw() const {
  // TODO: こう言う書き方.
  for (auto layer : layer_stack_) {
    layer->DrawTo(*screen_);
  }
}

// #@@range_begin(layermgr_move)
void LayerManager::Move(unsigned int id, Vector2D<int> new_position) {
  FindLayer(id)->Move(new_position);
}

void LayerManager::MoveRelative(unsigned int id, Vector2D<int> pos_diff) {
  FindLayer(id)->MoveRelative(pos_diff);
}

// 指定されたidを持つlayerを、new_heightの高さで書き換えるように、layer_stack_をいじる,
void LayerManager::UpDown(unsigned int id, int new_height) {
  if (new_height < 0) {
    Hide(id);
    return;
  }

  // new_heightは高すぎる値を入れても意味がない.
  if (new_height > layer_stack_.size()) {
    new_height = layer_stack_.size();
  }

  auto layer = FindLayer(id);
  auto old_pos = std::find(layer_stack_.begin(), layer_stack_.end(), layer);
  auto new_pos = layer_stack_.begin() + new_height;

  if (old_pos == layer_stack_.end()) {
    layer_stack_.insert(new_pos, layer);
    return;
  }

  if (new_pos == layer_stack_.end()) {
    --new_pos;
  }
  // layler_stackから古いレイヤーを削除.
  layer_stack_.erase(old_pos);
  // 新しいlayerを挿入.
  layer_stack_.insert(new_pos, layer);
}

void LayerManager::Hide(unsigned int id) {
  auto layer = FindLayer(id);
  auto pos = std::find(layer_stack_.begin(), layer_stack_.end(), layer);
  if (pos != layer_stack_.end()) {
    layer_stack_.erase(pos);
  }
}
// #@@range_end(layermgr_hide)

// #@@range_begin(layermgr_findlayer)
Layer* LayerManager::FindLayer(unsigned int id) {
  auto pred = [id](const std::unique_ptr<Layer>& elem) {
    return elem->ID() == id;
  };
  auto it = std::find_if(layers_.begin(), layers_.end(), pred);
  if (it == layers_.end()) {
    return nullptr;
  }
  return it->get();
}
// #@@range_end(layermgr_findlayer)

LayerManager* layer_manager;
