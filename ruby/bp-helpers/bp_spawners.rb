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

      fo = helper.new_funder_organization(name: 'Funder0')
      foo = helper.new_funder_organization_office(
	funder_organization: fo
      )
      foom = helper.new_funder_organization_office_member(
	funder_organization_office: foo, 
	user: f,
	can_manage_financial_accounts: true
      )

      foo.update(primary_contact_user: f)

      proj_params = {
	name: 'Manual Project',
	project_number: '0x01',
	street1: 'First Street',
	street2: 'Second Street',
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
	payment_type: 'manual'
      }

      manual_project = helper.new_project(proj_params)

      proj_params[:name] = 'Funded Manual Project'
      proj_params[:project_number] = '0x02'

      funded_manual_project = helper.new_project(proj_params)

      proj_params[:name] = 'Unused Project'
      proj_params[:project_number] = '0x03'
      proj_params[:street1] = 'Important Street'
      proj_params[:street2] = 'Less Important Street'
      proj_params[:payment_type] = nil

      free_project = helper.new_project(proj_params)

      p1rsp = make_psp(manual_project, 0)
      make_children(manual_project, p1rsp, 10)

      p2rsp = make_psp(funded_manual_project, 0)
      make_children(funded_manual_project, p2rsp, 10)

      p3rsp = make_psp(free_project, 0)
      make_children(free_project, p3rsp, 10)

      # merchant 0
      m_params = default_merchant_params
      ml_params = default_merchant_location_params
      mlmu_params = default_merchant_location_member_user_params
      setup_merchant(m_params, ml_params, mlmu_params)

      # merchant 1
      m_params = default_merchant_params(name: 'Merchant1', merchant_id: '0x01')
      mlmu_params = default_merchant_location_member_user_params(first_name: 'Merchant1', email: 'm1-l0@a.com')
      setup_merchant(m_params, ml_params, mlmu_params)

      # merchant 2
      m_params = default_merchant_params(name: 'Merchant2', merchant_id: '0x02')
      mlmu_params = default_merchant_location_member_user_params(first_name: 'Merchant2', email: 'm2-l0@a.com')
      setup_merchant(m_params, ml_params, mlmu_params)

      # merchant 3
      m_params = default_merchant_params(name: 'Merchant3', merchant_id: '0x03')
      mlmu_params = default_merchant_location_member_user_params(first_name: 'Merchant3', email: 'm3-l0@a.com')
      setup_merchant(m_params, ml_params, mlmu_params)

      ProjectStarter.new(manual_project).start
      ProjectStarter.new(funded_manual_project).start

      seed_funds(pm_from_psp(p2rsp), multiplier: 100_000, approve_labor: true, approve_material: true)
      p2rsp.children.each do |child|
	seed_funds(pm_from_psp(child), multiplier: 100, approve_labor: true, approve_material: true)
      end
    end
    nil
  end

  def make_psp(project, id, parent: nil)
    helper.new_project_provider(
      project: project,
      parent: parent,
      user: helper.new_user(
	first_name: "Worker#{id}",
	last_name: "Level#{level_id(parent)}",
	email: "w#{id}-l#{level_id(parent)}-p#{project.id}@a.com",
	home_mobile_phone: '9999999999',
	company_name: "Worker-#{id}-#{level_id(parent)}-#{project.id} Co",
	home_street1: 'Street1',
	home_street2: 'Street2',
	home_city: 'City',
	home_state: 'NY',
	home_postal_code: '12184',
	password: ' ',
	registered_at: 3.days.ago
      )
    )
  end

  def seed_funds(project_member, multiplier: 1, approve_labor: false, approve_material: false)
    to = project_member.user
    from = project_member.parent_user
    project = project_member.project

    labor = helper.new_labor_commitment(
      project: project,
      from_user: from,
      to_user: to,

      labor_amount: 1 * multiplier,
      overhead_amount: 1 * multiplier,
      profit_amount: 1 * multiplier,
      amount: 3 * multiplier
    )

    if approve_labor
      labor.acknowledge!
    else
      CommittedLaborFundsNotificationCreator.new(labor).create
    end

    material = helper.new_material_commitment(
      project: project,
      from_user: from,
      to_user: to,

      amount: 1 * multiplier
    )

    if approve_material
      material.acknowledge!
    else
      CommittedMaterialFundsNotificationCreator.new(material).create
    end
  end

  def make_children(project, parent_project_service_provider, num)
    num.times do |x|
      make_psp(project, x, parent: parent_project_service_provider)
    end
  end

  def create_invoices_for(*project_service_providers, parent: nil)
    project_service_providers.each do |psp|
      project = psp.project
      user = psp.user
      invoice = helper.new_invoice(project: project, user: user, parent: parent)
      helper.new_payout(invoice: invoice)
    end
  end

  def helper
    @helper ||= BuildpayTestHelper.new
  end

  def level_id(parent)
    parent.present? ? parent.id : '0'
  end

  def pm(project, user)
    ProjectMembers::Provider.build(project, user)
  end

  def pm_from_psp(psp) 
    ProjectMembers::Provider.build(psp.project, psp.user)
  end

  def setup_merchant(m_params, ml_params, mlmu_params)
    m = helper.new_merchant(m_params)

    ml_params[:merchant] = m

    ml = helper.new_merchant_location(ml_params)

    mlmu = helper.new_user(mlmu_params)

    helper.new_merchant_location_member(
      merchant_location: ml,
      user: mlmu,
      can_manage_financial_accounts: true
    )
  end

  def default_merchant_params(attrs = {})
    {
      name: 'Merchant',
      merchant_id: '0x00',
      street1: 'Street',
      city: 'City',
      state: 'NY',
      postal_code: '12184',
      primary_phone: '9999999999'
    }.merge(attrs)
  end

  def default_merchant_location_params(attrs = {})
    {
      name: 'Location',
      store_number: '0x00',
      street1: 'Street',
      city: 'City',
      state: 'NY',
      postal_code: '12184',
      primary_phone: '9999999999'
    }.merge(attrs)
  end

  def default_merchant_location_member_user_params(attrs = {})
    {
      first_name: 'Merchant0',
      last_name: 'Level0',
      email: 'm0-l0@a.com',
      home_mobile_phone: '9999999999',
      company_name: 'Merchant Co',
      home_street1: 'Street',
      home_city: 'City',
      home_state: 'NY',
      home_postal_code: '12184',
      password: ' ',
      registered_at: 3.days.ago
    }.merge(attrs)
  end
end

$G = Generator.new

