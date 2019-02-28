class Customers::FundingSourcesController < ApplicationController
  def list
    render json: resp, status: :ok
  end

  def create
    pp params
    render json: resp, status: :ok
  end

  private

  def resp
    {
      funding_source_token: "something"
    }
  end
end