Rails.application.routes.draw do
  resources :customers
  scope module: :customers do
    resource :funding_sources, path: '/customers/:customer_id/funding-sources'
  end
end
