class RouteRecognizer
  attr_reader :paths

  # To use this inside your app, call:
  # `RouteRecognizer.new.initial_path_segments`
  # This returns an array, e.g.: ['assets','blog','team','faq','users']

  INITIAL_SEGMENT_REGEX = %r{^\/([^\/\(:]+)}

  def initialize
    routes = Rails.application.routes.routes
    @paths = routes.collect {|r| r.path.spec.to_s }
  end

  def initial_path_segments
    @initial_path_segments ||= begin
      paths.collect {|path| match_initial_path_segment(path)}.compact.uniq
    end
  end

  def match_initial_path_segment(path)
    if match = INITIAL_SEGMENT_REGEX.match(path)
      match[1]
    end
  end
end

class Generator
  def seed
    ActiveRecord::Base.transaction do
      f = helper.new_funder_user(
	first_name: 'Funder1',
	last_name: 'Level1',
	email: 'f1-l1@a.com',
	home_mobile_phone: '9999999999',
	company_name: 'Funder Co',
	home_street1: 'Street',
	home_city: 'City',
	home_state: 'NY',
	home_postal_code: '12184',
	password: ' ',
	registered_at: 1.day.ago
      )

      helper.new_invite(user: f, created_by_user: User.second, email: f.email)

      o = helper.new_user(
	first_name: 'Owner',
	last_name: 'User',
	email: 'o@a.com',
	home_mobile_phone: '9999999999',
	company_name: 'Owner Co',
	home_street1: 'Street',
	home_city: 'City',
	home_state: 'NY',
	home_postal_code: '12184',
	password: ' ',
	registered_at: 2.days.ago
      )

      helper.new_invite(user: o, created_by_user: f, email: o.email)

      fo = helper.new_funder_organization(name: 'Funder0')
      foo = helper.new_funder_organization_office(
	funder_organization: fo
      )
      foom = helper.new_funder_organization_office_member(
	funder_organization_office: foo, 
	user: f,
	can_manage_bank_accounts: true
      )

      foo.update(primary_contact_user: f)

      manual_project = helper.new_project(
	name: 'Project0',
	project_number: '0x01',
	street1: 'Street',
	city: 'City',
	state: 'NY',
	postal_code: '12184',
	material_funds: 100_000_000,
	labor_funds: 100_000_000,
	overhead_funds: 100_000_000,
	profit_funds: 100_000_000,
	funder_user: f,
	funder_organization: fo,
	funder_organization_office: foo,
	funder_organization_office_member: foom,
	owner_user: o,
	payment_type: 'manual',
	started_at: DateTime.now
      )

      free_project = helper.new_project(
	name: 'Project1',
	project_number: '0x02',
	street1: 'Street',
	city: 'City',
	state: 'NY',
	postal_code: '12184',
	material_funds: 100_000_000,
	labor_funds: 100_000_000,
	overhead_funds: 100_000_000,
	profit_funds: 100_000_000,
	funder_user: f,
	funder_organization: fo,
	funder_organization_office: foo,
	funder_organization_office_member: foom,
	owner_user: o,
	payment_type: nil,
      )

      w1 = helper.new_user(
	first_name: 'Worker1',
	last_name: 'Level1',
	email: 'w1-l1@a.com',
	home_mobile_phone: '9999999999',
	company_name: 'Worker Co',
	home_street1: 'Street',
	home_city: 'City',
	home_state: 'NY',
	home_postal_code: '12184',
	password: ' ',
	registered_at: 3.days.ago
      )

      helper.new_invite(user: w1, created_by_user: o, email: w1.email)

      w2 = helper.new_user(
	first_name: 'Worker1',
	last_name: 'Level2',
	email: 'w1-l2@a.com',
	home_mobile_phone: '9999999999',
	company_name: 'Worker Co',
	home_street1: 'Street',
	home_city: 'City',
	home_state: 'NY',
	home_postal_code: '12184',
	password: ' ',
	registered_at: 3.days.ago
      )

      helper.new_invite(user: w2, created_by_user: w1, email: w2.email)

      p1sp1 = helper.new_project_provider(
	project: manual_project,
	user: w1
      )

      p1sp2 = helper.new_project_provider(
	project: manual_project,
	user: w2,
	parent: p1sp1
      )

      p2sp1 = helper.new_project_provider(
	project: free_project,
	user: w1
      )

      p2sp2 = helper.new_project_provider(
	project: free_project,
	user: w2,
	parent: p2sp1
      )
    end
  end

  private

  def helper
    @helper ||= BuildpayTestHelper.new
  end
end
